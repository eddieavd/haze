//
//
//      natprolib
//      segment_tree_new.hpp
//

#pragma once

#include "../util/util.h"
#include "../util/traits.h"
#include "../util/mem.h"
#include "../util/compressed_pair.h"
#include "../util/split_buffer.h"


namespace npl
{

namespace rq
{


template< typename T, typename Allocator >
class _segment_tree_base
{
public:
        using  allocator_type = Allocator;
        using   _alloc_traits = std::allocator_traits< allocator_type >;
        using       size_type = typename _alloc_traits::size_type;
protected:
        using      value_type = T;
        using difference_type = typename _alloc_traits::difference_type;
        using       reference = value_type &;
        using const_reference = value_type const &;
        using         pointer = typename _alloc_traits::pointer;
        using   const_pointer = typename _alloc_traits::const_pointer;
        using        iterator = pointer;
        using  const_iterator = const_pointer;

        pointer begin_;
        pointer end_;
        compressed_pair< pointer, allocator_type > end_cap_;

        allocator_type       & _alloc ()       noexcept { return end_cap_.second(); }
        allocator_type const & _alloc () const noexcept { return end_cap_.second(); }

        pointer       & _end_cap ()       noexcept { return end_cap_.first(); }
        pointer const & _end_cap () const noexcept { return end_cap_.first(); }

        _segment_tree_base (                             ) noexcept( std::is_nothrow_default_constructible_v< allocator_type > );
        _segment_tree_base ( allocator_type const &  _a_ );
        _segment_tree_base ( allocator_type       && _a_ ) noexcept;

        ~_segment_tree_base ();

        void clear () noexcept { _destruct_at_end( begin_ ); }

        size_type capacity () const noexcept
        { return static_cast< size_type >( _end_cap() - begin_ ); }

        void _destruct_at_end ( pointer _new_last_ ) noexcept;

        void _copy_assign_alloc ( _segment_tree_base const & _c_ )
        {
                _copy_assign_alloc( _c_, std::integral_constant< bool,
                                _alloc_traits::propagate_on_container_copy_assignment::value >() );
        }

        void _move_assign_alloc ( _segment_tree_base & _c_ )
                noexcept(
                                !_alloc_traits::propagate_on_container_move_assignment::value ||
                                std::is_nothrow_move_assignable_v< allocator_type >
                        )
        {
                _move_assign_alloc( _c_, std::integral_constant< bool,
                                _alloc_traits::propagate_on_container_move_assignment::value >() );
        }

        [[ noreturn ]] void _throw_length_error () const
        {
                std::abort();
        }

        [[ noreturn ]] void _throw_out_of_range () const
        {
                std::abort();
        }

private:
        void _copy_assign_alloc ( _segment_tree_base const & _c_, std::true_type )
        {
                if( _alloc() != _c_._alloc() )
                {
                        clear();
                        _alloc_traits::deallocate( _alloc(), begin_, capacity() );
                        begin_ = end_ = end_cap_() = nullptr;
                }
                _alloc() = _c_._alloc();
        }

        void _copy_assign_alloc ( _segment_tree_base const &, std::false_type ) {}

        void _move_assign_alloc ( _segment_tree_base & _c_, std::true_type )
                noexcept( std::is_nothrow_move_assignable_v< allocator_type > )
        {
                _alloc() = NPL_MOVE( _c_._alloc() );
        }

        void _move_assign_alloc ( _segment_tree_base &, std::false_type ) noexcept {}
};

template< typename T, class Allocator >
inline
void _segment_tree_base< T, Allocator >::_destruct_at_end ( pointer _new_last_ ) noexcept
{
        pointer soon_to_be_end = end_;
        while( _new_last_ != soon_to_be_end )
        {
                _alloc_traits::destroy( _alloc(), std::to_address( --soon_to_be_end ) );
        }
        end_ = _new_last_;
}

template< typename T, typename Allocator >
inline
_segment_tree_base< T, Allocator >::_segment_tree_base ()
        noexcept( std::is_nothrow_default_constructible_v< allocator_type > )
        : begin_  ( nullptr ),
          end_    ( nullptr ),
          end_cap_( nullptr, _default_init_tag() ) {}

template< typename T, typename Allocator >
inline
_segment_tree_base< T, Allocator >::_segment_tree_base ( allocator_type const & _a_ )
        : begin_  ( nullptr ),
          end_    ( nullptr ),
          end_cap_( nullptr, _a_ ) {}

template< typename T, typename Allocator >
inline
_segment_tree_base< T, Allocator >::_segment_tree_base ( allocator_type && _a_ ) noexcept
        : begin_  ( nullptr ),
          end_    ( nullptr ),
          end_cap_( nullptr, std::move( _a_ ) ) {}

template< typename T, typename Allocator >
_segment_tree_base< T, Allocator >::~_segment_tree_base ()
{
        if( begin_ != nullptr )
        {
                clear();
                _alloc_traits::deallocate( _alloc(), begin_, capacity() );
        }
}


template< typename T >
auto pb_default
{
        []( [[ maybe_unused ]] T const & lhs, [[ maybe_unused ]] T const & rhs )
        {
                return T();
        }
};


template< typename T, auto _pb_ = pb_default< T >, typename Allocator = std::allocator< T > >
class segment_tree;

template< typename T, bool C, typename Allocator = std::allocator< T > >
class segment_tree_iterator
{

};

template< typename T, auto _pb_, typename Allocator >
class segment_tree
        : private _segment_tree_base< T, Allocator >
{
private:
        using _base                   = _segment_tree_base< T, Allocator >;
        using _default_allocator_type = std::allocator< T >;
        using _parent_builder_type    = decltype( _pb_ );
public:
        using           _self = segment_tree;
        using      value_type = T;
        using  allocator_type = Allocator;
        using   _alloc_traits = typename _base::_alloc_traits;
        using       reference = typename _base::reference;
        using const_reference = typename _base::const_reference;
        using       size_type = typename _base::size_type;
        using difference_type = typename _base::difference_type;
        using         pointer = typename _base::pointer;
        using   const_pointer = typename _base::const_pointer;

        using               iterator = typename _base::iterator;
        using         const_iterator = typename _base::const_iterator;
        using       reverse_iterator = std::reverse_iterator<       iterator >;
        using const_reverse_iterator = std::reverse_iterator< const_iterator >;

        static_assert( ( std::is_same_v< typename allocator_type::value_type, value_type > ),
                        "Allocator::value_type != value_type" );

        static_assert( ( std::is_same_v< T, decltype( _pb_( T(), T() ) ) > ),
                        "Bad parent builder" );

        segment_tree () noexcept( std::is_nothrow_default_constructible_v< allocator_type > );

        explicit segment_tree ( allocator_type const & _a_ ) noexcept;

        explicit segment_tree ( size_type _n_                             );
        explicit segment_tree ( size_type _n_, allocator_type const & _a_ );

        segment_tree ( size_type _n_, value_type const & _x_                             );
        segment_tree ( size_type _n_, value_type const & _x_, allocator_type const & _a_ );

        template< typename InputIterator, typename = std::enable_if_t< !std::is_same_v< typename std::iterator_traits< InputIterator >::value_type, void > > >
        segment_tree ( InputIterator _first_,
                        typename std::enable_if_t<  is_cpp17_input_iterator_v< InputIterator   > &&
                                                   !is_cpp17_forward_iterator_v< InputIterator > &&
                                                    std::is_constructible_v<
                                                        value_type,
                                                        typename std::iterator_traits< InputIterator >::reference
                                                >,
                                                InputIterator > _last_ );

        template< typename InputIterator, typename = std::enable_if_t< !std::is_same_v< typename std::iterator_traits< InputIterator >::value_type, void > > >
        segment_tree ( InputIterator _first_, InputIterator _last_, allocator_type const & _a_,
                        typename std::enable_if_t<  is_cpp17_input_iterator_v< InputIterator   > &&
                                                   !is_cpp17_forward_iterator_v< InputIterator > &&
                                                    std::is_constructible_v<
                                                        value_type,
                                                        typename std::iterator_traits< InputIterator >::reference
                                                > > * = 0 );

        template< typename ForwardIterator, typename = std::enable_if_t< !std::is_same_v< typename std::iterator_traits< ForwardIterator >::value_type, void > > >
        segment_tree ( ForwardIterator _first_,
                        typename std::enable_if_t< is_cpp17_forward_iterator_v< ForwardIterator > &&
                                                   std::is_constructible_v<
                                                        value_type,
                                                        typename std::iterator_traits< ForwardIterator >::reference
                                                >,
                                                ForwardIterator > _last_ );

        template< typename ForwardIterator, typename = std::enable_if_t< !std::is_same_v< typename std::iterator_traits< ForwardIterator >::value_type, void > > >
        segment_tree ( ForwardIterator _first_, ForwardIterator _last_, allocator_type const & _a_,
                        typename std::enable_if_t< is_cpp17_forward_iterator_v< ForwardIterator > &&
                                                   std::is_constructible_v<
                                                        value_type,
                                                        typename std::iterator_traits< ForwardIterator >::reference
                                                > > * = 0 );

        ~segment_tree ()
        {
                _annotate_delete();
        }

        segment_tree ( segment_tree const & _x_                                          );
        segment_tree ( segment_tree const & _x_, _identity< allocator_type > const & _a_ );

        segment_tree & operator= ( segment_tree const & _x_ );

        segment_tree ( std::initializer_list< value_type > _list_                             );
        segment_tree ( std::initializer_list< value_type > _list_, allocator_type const & _a_ );

        segment_tree ( segment_tree && _x_                                          ) noexcept;
        segment_tree ( segment_tree && _x_, _identity< allocator_type > const & _a_ );

        segment_tree & operator= ( segment_tree && _x_ )
                noexcept( ( noexcept_move_assign_container_v< Allocator, _alloc_traits > ) );

        segment_tree & operator= ( std::initializer_list< value_type > _list_ )
        { assign( _list_.begin(), _list_.end() ); return *this; }

        template< typename InputIterator, typename = std::enable_if_t< !std::is_same_v< typename std::iterator_traits< InputIterator >::value_type, void > > >
        typename std::enable_if_t
        <
                 is_cpp17_input_iterator_v< InputIterator    > &&
                !is_cpp17_forward_iterator_v< InputIterator > &&
                 std::is_constructible_v<
                        value_type,
                        typename std::iterator_traits< InputIterator >::reference
                >,
                void
        >
        assign ( InputIterator _first_, InputIterator _last_ );

        template< typename ForwardIterator, typename = std::enable_if_t< !std::is_same_v< typename std::iterator_traits< ForwardIterator >::value_type, void > > >
        typename std::enable_if_t
        <
                is_cpp17_forward_iterator_v< ForwardIterator > &&
                std::is_constructible_v<
                        value_type,
                        typename std::iterator_traits< ForwardIterator >::reference
                >,
                void
        >
        assign ( ForwardIterator _first_, ForwardIterator _last_ );

        void assign ( size_type _n_, const_reference _x_ );

        void assign ( std::initializer_list< value_type > _list_ )
        { assign( _list_.begin(), _list_.end() ); }

        allocator_type get_allocator () const noexcept
        { return this->_alloc(); }

        const_iterator begin () const noexcept;
        const_iterator end   () const noexcept;

        const_reverse_iterator rbegin () const noexcept
        { return const_reverse_iterator( begin() ); }

        const_reverse_iterator rend () const noexcept
        { return const_reverse_iterator( end() ); }

        const_iterator cbegin () const noexcept
        { return begin(); }

        const_iterator cend () const noexcept
        { return end(); }

        const_reverse_iterator crbegin () const noexcept
        { return rbegin(); }

        const_reverse_iterator crend () const noexcept
        { return rend(); }

        size_type size () const noexcept
        { return _size() - ( capacity_ / 2 ); }

        size_type capacity () const noexcept
        { return _capacity() / 2; }

        [[ nodiscard ]]
        bool empty () const noexcept
        { return size() == 0; }

        size_type max_size () const noexcept;

        void reserve ( size_type _n_ );
        void shrink_to_fit () noexcept;

        const_reference operator[] ( size_type _n_ ) const noexcept;

        bool   operator== ( segment_tree< T, _pb_, Allocator > const & _rhs_ ) const noexcept;
        auto   operator+  ( segment_tree< T, _pb_, Allocator > const & _rhs_ ) const         ;
        auto & operator+= ( segment_tree< T, _pb_, Allocator > const & _rhs_ )       noexcept;
        auto   operator-  ( segment_tree< T, _pb_, Allocator > const & _rhs_ ) const         ;
        auto & operator-= ( segment_tree< T, _pb_, Allocator > const & _rhs_ )       noexcept;

        const_reference         at ( size_type _n_ ) const noexcept;
        const_reference element_at ( size_type _n_ ) const noexcept;

        value_type range (                              ) const noexcept;
        value_type range ( size_type _x_, size_type _y_ ) const noexcept;

        void update ( size_type _n_, const_reference _x_ );
        void update ( size_type _n_, value_type &&   _x_ ) noexcept;

        template< typename... Args >
        void update ( size_type _n_, Args... _args_ ) noexcept;


        // 2d overloads
        template< typename U = _self >
        enable_2d_container_base_t< U > const &
        at ( size_type _x_, size_type _y_ ) const noexcept
        {
                return at( _x_ ).at( _y_ );
        }

        template< typename U = _self >
        enable_2d_range_container_base_t< U > const &
        element_at ( size_type _x_, size_type _y_ ) const noexcept
        {
                return at( _x_, _y_ );
        }

        template< typename U = _self >
        enable_2d_range_container_base_t< U >
        range ( size_type _x1_, size_type _y1_, size_type _x2_, size_type _y2_ ) const noexcept
        {
                return range( _x1_, _x2_ ).range( _y1_, _y2_ );

                if( _x1_ == 0 )
                {
                        return at( _x2_ ).range( _y1_, _y2_ );
                }
                else
                {
                        return at( _x2_ ).range( _y1_, _y2_ ) - at( _x1_ - 1 ).range( _y1_, _y2_ );
                }
        }
        //

        // 3d overloads
        template< typename U = _self >
        enable_3d_container_base_t< U > const &
        at ( size_type _x_, size_type _y_, size_type _z_ ) const noexcept
        {
                return at( _x_ ).at( _y_ ).at( _z_ );
        }

        template< typename U = _self >
        enable_3d_range_container_base_t< U > const &
        element_at ( size_type _x_, size_type _y_, size_type _z_ ) const noexcept
        {
                return at( _x_, _y_, _z_ );
        }

        template< typename U = _self >
        enable_3d_range_container_base_t< U >
        range ( size_type _x1_, size_type _y1_, size_type _z1_,
                        size_type _x2_, size_type _y2_, size_type _z2_ ) const noexcept
        {
                return range( _x1_, _x2_ ).range( _y1_, _y2_ ).range( _z1_, _z2_ );
                return range( _x1_, _x2_ ).range( _y1_, _z1_, _y2_, _z2_ );

                if( _x1_ == 0 )
                {
                        return at( _x2_ ).range( _y1_, _z1_, _y2_, _z2_ );
                }
                else
                {
                        return at( _x2_ ).range( _y1_, _z1_, _y2_, _z2_ ) - at( _x1_ - 1 ).range( _y1_, _z1_, _y2_, _z2_ );
                }
        }
        //

        const_reference front () const noexcept
        {
                NPL_ASSERT( !empty(), "segment_tree::front called on empty segment_tree" );

                return *( this->begin_ + ( capacity_ / 2 ) );   // NAH BRUH
        }

        const_reference back () const noexcept
        {
                NPL_ASSERT( !empty(), "segment_tree::back called on empty segment_tree" );

                return *( this->end_ - 1 );
        }

        value_type const * data () const noexcept
        { return std::to_address( this->begin_ ); }

        template< typename Arg >
        void _emplace_back ( Arg&& _arg_ )
        {
                emplace_back( NPL_FWD( _arg_ ) );
        }

        void push_back ( const_reference _x_ );

        template< typename... Args >
        void push_back ( const_reference _x_, Args... _args_ )
        {
                push_back( _x_ );
                push_back( _args_... );
        }

        void push_back ( value_type && _x_ );

        template< typename... Args >
        void push_back ( value_type && _x_, Args... _args_ )
        {
                push_back( _x_ );
                push_back( _args_... );
        }

        template< typename... Args >
        const_reference emplace_back ( Args&&... _args_ );

        void pop_back ();

private:
        //  some more inserts to implement

public:
        void clear () noexcept
        {
                size_type old_size = size();
                _base::clear();
                _annotate_shrink( old_size );
                _invalidate_all_iterators();
        }

        bool _invariants () const;

private:
        _parent_builder_type pb_{ _pb_ };
        size_type capacity_{ 0 };

        void _invalidate_all_iterators ();
        void _invalidate_iterators_past ( pointer _new_last_ );

        const_reference _at ( size_type _n_ ) const noexcept;

        value_type _range ( size_type _x_, size_type _y_ ) const noexcept;

        void _rebuild_tree              () noexcept;
        void _restructure_tree_forward  () noexcept;
        void _restructure_tree_backward () noexcept;

        void _vallocate   ( size_type _n_ );
        void _vdeallocate (               ) noexcept;

        size_type _msb64          ( size_type _n_        ) const noexcept;
        size_type _extend_to_pow2 ( size_type _n_        ) const noexcept;
        size_type _recommend      ( size_type _new_size_ ) const noexcept;

        void _update ( size_type _n_, const_reference _x_ );
        void _update ( size_type _n_, value_type &&   _x_ ) noexcept;

        template< typename... Args >
        void _update ( size_type _n_, Args... _args_ ) noexcept;

        size_type _size () const noexcept
        { return static_cast< size_type >( this->end_ - this->begin_ ); }

        size_type _capacity () const noexcept
        { return _base::capacity(); }

        void _construct_at_end ( size_type _n_                      );
        void _construct_at_end ( size_type _n_, const_reference _x_ );

        template< typename ForwardIterator, typename = std::enable_if_t< !std::is_same_v< typename std::iterator_traits< ForwardIterator >::value_type, void > > >
        typename std::enable_if_t
        <
                is_cpp17_forward_iterator_v< ForwardIterator >,
                void
        >
        _construct_at_end ( ForwardIterator _first_, ForwardIterator _last_, size_type _n_ );

        void _append ( size_type _n_                      );
        void _append ( size_type _n_, const_reference _x_ );

              iterator _make_iter ( pointer _p_ )       noexcept;
        const_iterator _make_iter ( pointer _p_ ) const noexcept;

        void    _swap_out_circular_buffer ( split_buffer< value_type, allocator_type & > & _b_              );
        pointer _swap_out_circular_buffer ( split_buffer< value_type, allocator_type & > & _b_, pointer _p_ );

        void _move_range ( pointer _from_s_, pointer _from_e_, pointer _to_ );

        void _move_assign ( segment_tree & _c_, std::true_type  ) noexcept( std::is_nothrow_move_assignable_v< allocator_type > );
        void _move_assign ( segment_tree & _c_, std::false_type ) noexcept( _alloc_traits::is_always_equal::value );

        void _destruct_at_end ( pointer _new_last_ ) noexcept
        {
                _invalidate_iterators_past( _new_last_ );
                size_type old_size = size();
                _base::_destruct_at_end( _new_last_ );
                _annotate_shrink( old_size );
        }

        template< typename U >
        inline void _push_back_slow_path ( U&& _x_ );

        template< typename... Args >
        inline void _emplace_back_slow_path ( Args&&... _args_ );

#ifdef NPL_HAS_ASAN
        void _annotate_contiguous_container ( void const * _beg_, void const * _end_,
                                              void const * _old_mid_,
                                              void const * _new_mid_ ) const
        {
                if( _beg_ && std::is_same_v< allocator_type, _default_allocator_type > )
                {
                        std::__sanitizer_annotate_contiguous_container( _beg_, _end_, _old_mid_, _new_mid_ );
                }
        }
#else
        void _annotate_contiguous_container ( void const *, void const *, void const *, void const * ) const noexcept {}
#endif
        void _annotate_new ( size_type _current_size_ ) const noexcept
        {
                _annotate_contiguous_container( data(), data() + capacity(),
                                                data() + capacity(), data() + _current_size_ );
        }

        void _annotate_delete () const noexcept
        {
                _annotate_contiguous_container( data(), data() + capacity(),
                                                data() + size(), data() + capacity() );
        }

        void _annotate_increase ( size_type _n_ ) const noexcept
        {
                _annotate_contiguous_container( data(), data() + capacity(),
                                                data() + size(), data() + size() + _n_ );
        }

        void _annotate_shrink ( size_type _old_size_ ) const noexcept
        {
                _annotate_contiguous_container( data(), data() + capacity(),
                                                data() + _old_size_, data() + size() );
        }

        struct _construct_transaction
        {
                explicit _construct_transaction ( segment_tree & _s_, size_type _n_ )
                        : s_( _s_ ), pos_( _s_.end_ ), new_end_( _s_.end_ + _n_ )
                {
#ifdef NPL_HAS_ASAN
                        s_._annotate_increase( _n_ );
#endif
                }

                ~_construct_transaction ()
                {
                        s_.end_ = pos_;
#ifdef NPL_HAS_ASAN
                        if( pos_ != new_end_ )
                        {
                                s_._annotate_shrink( new_end_ - s_.begin_ );
                        }
#endif
                }

                segment_tree  & s_;
                pointer         pos_;
                const_pointer   new_end_;

        private:
                _construct_transaction             ( _construct_transaction const & ) = delete;
                _construct_transaction & operator= ( _construct_transaction const & ) = delete;
        };

        template< typename... Args >
        void _construct_one_at_end ( Args... _args_ )
        {
                _construct_transaction tx( *this, 1 );

                _alloc_traits::construct( this->_alloc(), std::to_address( tx.pos_ ), NPL_FWD( _args_ )... );
                ++tx.pos_;
        }
};

template< typename T, auto _pb_, typename Allocator >
void
segment_tree< T, _pb_, Allocator >::_swap_out_circular_buffer ( split_buffer< value_type, allocator_type & > & _b_ )
{
        _annotate_delete();

        mem::_construct_backward_with_exception_guarantees( this->_alloc(), this->begin_, this->end_, _b_.begin_ );

        std::swap( this->begin_    , _b_.begin_     );
        std::swap( this->end_      , _b_.end_       );
        std::swap( this->_end_cap(), _b_._end_cap() );

        _b_.first_ = _b_.begin_;
        _annotate_new( _size() ); // huh FIX
        _invalidate_all_iterators();
}

template< typename T, auto _pb_, typename Allocator >
typename segment_tree< T, _pb_, Allocator >::pointer
segment_tree< T, _pb_, Allocator >::_swap_out_circular_buffer ( split_buffer< value_type, allocator_type & > & _b_, pointer _p_ )
{
        _annotate_delete();

        pointer r = _b_.begin_;

        mem::_construct_backward_with_exception_guarantees( this->_alloc(), this->begin_, _p_, _b_.begin_ );
        mem::_construct_forward_with_exception_guarantees( this->_alloc(), _p_, this->end_, _b_.end_ );

        std::swap( this->begin_    , _b_.begin_     );
        std::swap( this->end_      , _b_.end_       );
        std::swap( this->_end_cap(), _b_._end_cap() );

        _b_.first_ = _b_.begin_;
        _annotate_new( _size() );  //  AYO BRUH FIX
        _invalidate_all_iterators();

        return r;
}

template< typename T, auto _pb_, typename Allocator >
void
segment_tree< T, _pb_, Allocator >::_rebuild_tree () noexcept
{
        for( size_type i = ( this->capacity_ / 2 ) - 1; i > 0; --i )
        {
                auto lchild = 2 * i;
                auto rchild = lchild + 1;

                if( lchild >= _size() ) // FIX what size is this bruh
                {
                        this->begin_[ i ] = value_type();
                }
                else if( rchild >= _size() )
                {
                        this->begin_[ i ] = this->begin_[ lchild ];
                }
                else
                {
                        this->begin_[ i ] = this->pb_( this->begin_[ lchild ], this->begin_[ rchild ] );
                }
        }
}

template< typename T, auto _pb_, typename Allocator >
void
segment_tree< T, _pb_, Allocator >::_restructure_tree_forward () noexcept
{
        for( size_type i = 0; i < capacity_ / 4; ++i )
        {
                this->begin_[ capacity_ / 2 + i ] = NPL_MOVE( this->begin_[ capacity_ / 4 + i ] );
                this->end_++;
        }
}

template< typename T, auto _pb_, typename Allocator >
void
segment_tree< T, _pb_, Allocator >::_restructure_tree_backward () noexcept
{
        // kek
}

template< typename T, auto _pb_, typename Allocator >
void
segment_tree< T, _pb_, Allocator >::_vallocate ( size_type _n_ )
{
        if( _n_ > max_size() )
        {
                this->_throw_length_error();
        }

        this->begin_ = this->end_ = _alloc_traits::allocate( this->_alloc(), _n_ );
        this->_end_cap() = this->begin_ + _n_;

        _annotate_new( 0 );
}

template< typename T, auto _pb_, typename Allocator >
void
segment_tree< T, _pb_, Allocator >::_vdeallocate () noexcept
{
        if( this->begin_ != nullptr )
        {
                clear();
                _alloc_traits::deallocate( this->_alloc(), this->begin_, _capacity() );
                this->begin_ = this->end_ = this->_end_cap() = nullptr;
        }
}

template< typename T, auto _pb_, typename Allocator >
typename segment_tree< T, _pb_, Allocator >::size_type
segment_tree< T, _pb_, Allocator >::max_size () const noexcept
{
        return std::min< size_type >( _alloc_traits::max_size( this->_alloc() ), std::numeric_limits< difference_type >::max() );
}

template< typename T, auto _pb_, typename Allocator >
typename segment_tree< T, _pb_, Allocator >::size_type
segment_tree< T, _pb_, Allocator >::_msb64 ( size_type _n_ ) const noexcept
{
        _n_ |= ( _n_ >>  1 );
        _n_ |= ( _n_ >>  2 );
        _n_ |= ( _n_ >>  4 );
        _n_ |= ( _n_ >>  8 );
        _n_ |= ( _n_ >> 16 );
        if( sizeof( size_type ) == 8 )
        {
                _n_ |= ( _n_ >> 32 );
        }

        return ( _n_ & ~( _n_ >> 1 ) );
}

template< typename T, auto _pb_, typename Allocator >
typename segment_tree< T, _pb_, Allocator >::size_type
segment_tree< T, _pb_, Allocator >::_extend_to_pow2 ( size_type _n_ ) const noexcept
{
        size_type msb = _msb64( _n_ );

        if( _n_ == msb )
        {
                return _n_;
        }
        else
        {
                return ( msb << 1 );
        }
}

template< typename T, auto _pb_, typename Allocator >
inline
typename segment_tree< T, _pb_, Allocator >::size_type
segment_tree< T, _pb_, Allocator >::_recommend ( size_type _new_size_ ) const noexcept
{
        size_type const ms = max_size();

        NPL_ASSERT( _new_size_ < ms, "segment_tree: new size > max_size" );

        size_type const cap = _capacity();

        if( cap >= ms / 2 )
        {
                return ms;
        }

        return std::max< size_type >( 2 * cap, _new_size_ );
}

template< typename T, auto _pb_, typename Allocator >
void
segment_tree< T, _pb_, Allocator >::_update ( size_type _n_, const_reference _x_ )
{
        _n_ += this->capacity_ / 2;

        this->begin_[ _n_ ] = _x_;

        for( _n_ /= 2; _n_ >= 1; _n_ /= 2 )
        {
                size_type lchild = 2 * _n_;
                size_type rchild = lchild + 1;

                if( lchild >= _size() )
                {
                        this->begin_[ _n_ ] = value_type();
                }
                else if( rchild >= _size() )
                {
                        this->begin_[ _n_ ] = this->begin_[ lchild ];
                }
                else
                {
                        this->begin_[ _n_ ] = this->pb_( this->begin_[ lchild ], this->begin_[ rchild ] );
                }
        }
}

template< typename T, auto _pb_, typename Allocator >
void
segment_tree< T, _pb_, Allocator >::_update ( size_type _n_, value_type && _x_ ) noexcept
{
        _n_ += this->capacity_ / 2;

        this->begin_[ _n_ ] = NPL_MOVE( _x_ );

        for( _n_ /= 2; _n_ >= 1; _n_ /= 2 )
        {
                size_type lchild = 2 * _n_;
                size_type rchild = lchild + 1;

                if( lchild >= _size() )
                {
                        this->begin_[ _n_ ] = value_type();
                }
                else if( rchild >= _size() )
                {
                        this->begin_[ _n_ ] = this->begin_[ lchild ];
                }
                else
                {
                        this->begin_[ _n_ ] = this->pb_( this->begin_[ lchild ], this->begin_[ rchild ] );
                }
        }
}

template< typename T, auto _pb_, typename Allocator >
template< typename... Args >
void
segment_tree< T, _pb_, Allocator >::_update ( size_type _n_, Args... _args_ ) noexcept
{
        _n_ += this->capacity_ / 2;

        this->begin_[ _n_ ] = value_type( _args_... );

        for( _n_ /= 2; _n_ >= 1; _n_ /= 2 )
        {
                size_type lchild = 2 * _n_;
                size_type rchild = lchild + 1;

                if( lchild >= _size() )
                {
                        this->begin_[ _n_ ] = value_type();
                }
                else if( rchild >= _size() )
                {
                        this->begin_[ _n_ ] = this->begin_[ lchild ];
                }
                else
                {
                        this->begin_[ _n_ ] = this->pb_( this->begin_[ lchild ], this->begin_[ rchild ] );
                }
        }
}

template< typename T, auto _pb_, typename Allocator >
void
segment_tree< T, _pb_, Allocator >::_construct_at_end ( size_type _n_ )
{
        _construct_transaction tx( *this, _n_ );
        const_pointer new_end = tx.new_end_;

        for( pointer pos = tx.pos_; pos != new_end; ++pos, tx.pos_ = pos )
        {
                _alloc_traits::construct( this->_alloc(), std::to_address( pos ) );
        }
}

template< typename T, auto _pb_, typename Allocator >
inline
void
segment_tree< T, _pb_, Allocator >::_construct_at_end ( size_type _n_, const_reference _x_ )
{
        _construct_transaction tx( *this, _n_ );
        const_pointer new_end = tx.new_end_;

        for( pointer pos = tx.pos_; pos != new_end; ++pos, tx.pos_ = pos )
        {
                _alloc_traits::construct( this->_alloc(), std::to_address( pos ), _x_ );
        }
}

template< typename T, auto _pb_, typename Allocator >
template< typename ForwardIterator, typename U >
typename std::enable_if_t
<
        is_cpp17_forward_iterator_v< ForwardIterator >,
        void
>
segment_tree< T, _pb_, Allocator >::_construct_at_end ( ForwardIterator _first_, ForwardIterator _last_, size_type _n_ )
{
        _construct_transaction tx( *this, _n_ );
        const_pointer new_end = tx.new_end_;

        for( pointer pos = tx.pos_; pos != new_end && _first_ != _last_; ++pos, tx.pos_ = pos )
        {
                _alloc_traits::construct( this->_alloc(), std::to_address( pos ), *_first_++ );
        }
}

template< typename T, auto _pb_, typename Allocator >
void
segment_tree< T, _pb_, Allocator >::_append ( size_type _n_ )
{
        if( static_cast< size_type >( this->_end_cap() - this->end_ ) >= _n_ )
        {
                this->_construct_at_end( _n_ );
        }
        else
        {
                allocator_type & a = this->_alloc();
                split_buffer< value_type, allocator_type & > b( _recommend( size() + _n_ ), size(), a );
                b._construct_at_end( _n_ );
                _swap_out_circular_buffer( b );
        }
}

template< typename T, auto _pb_, typename Allocator >
void
segment_tree< T, _pb_, Allocator >::_append ( size_type _n_, const_reference _x_ )
{
        if( static_cast< size_type >( this->_end_cap() - this->end_ ) >= _n_ )
        {
                this->_construct_at_end( _n_, _x_ );
        }
        else
        {
                allocator_type & a = this->_alloc();
                split_buffer< value_type, allocator_type & > b( _recommend( size() + _n_ ), size(), a );
                b._construct_at_end( _n_, _x_ );
                _swap_out_circular_buffer( b );
        }
}

template< typename T, auto _pb_, typename Allocator >
segment_tree< T, _pb_, Allocator >::segment_tree ()
        noexcept( std::is_nothrow_default_constructible_v< allocator_type > )
        : capacity_( NPL_RQ_DEFAULT_CAPACITY )
{
        _vallocate( capacity_ );
        _construct_at_end( capacity_ / 2 );
}

template< typename T, auto _pb_, typename Allocator >
segment_tree< T, _pb_, Allocator >::segment_tree ( size_type _n_ )
{
        if( _n_ > 0 )
        {
                auto size = _extend_to_pow2( _n_ );
                capacity_ = size * 2;

                _vallocate( capacity_ );
                _construct_at_end( size );
        }
}

template< typename T, auto _pb_, typename Allocator >
segment_tree< T, _pb_, Allocator >::segment_tree ( size_type _n_, allocator_type const & _a_ )
        : _base( _a_ )
{
        if( _n_ > 0 )
        {
                auto size = _extend_to_pow2( _n_ );
                capacity_ = size * 2;

                _vallocate( capacity_ );
                _construct_at_end( size );
        }
}

template< typename T, auto _pb_, typename Allocator >
segment_tree< T, _pb_, Allocator >::segment_tree ( size_type _n_, value_type const & _x_ )
{
        if( _n_ > 0 )
        {
                auto size = _extend_to_pow2( _n_ );
                capacity_ = size * 2;

                _vallocate( capacity_ );
                _construct_at_end( size );
                _construct_at_end( _n_, _x_ );
                _rebuild_tree();
        }
}

template< typename T, auto _pb_, typename Allocator >
segment_tree< T, _pb_, Allocator >::segment_tree ( size_type _n_, value_type const & _x_, allocator_type const & _a_ )
        : _base( _a_ )
{
        if( _n_ > 0 )
        {
                auto size = _extend_to_pow2( _n_ );
                capacity_ = size * 2;

                _vallocate( capacity_ );
                _construct_at_end( size );
                _construct_at_end( _n_, _x_ );
                _rebuild_tree();
        }
}

template< typename T, auto _pb_, typename Allocator >
template< typename InputIterator, typename U >
segment_tree< T, _pb_, Allocator >::segment_tree ( InputIterator _first_,
                typename std::enable_if_t< is_cpp17_input_iterator_v< InputIterator > &&
                                          !is_cpp17_forward_iterator_v< InputIterator > &&
                                           std::is_constructible_v<
                                                value_type,
                                                typename std::iterator_traits< InputIterator >::reference
                                        >,
                                        InputIterator > _last_ )
{
        capacity_      = NPL_RQ_DEFAULT_CAPACITY * 2;
        size_type size = NPL_RQ_DEFAULT_CAPACITY;

        _vallocate( capacity_ );
        _construct_at_end( size );

        for( ; _first_ != _last_; ++_first_ )
        {
                _emplace_back( *_first_ );
        }

        _rebuild_tree();
}

template< typename T, auto _pb_, typename Allocator >
template< typename InputIterator, typename U >
segment_tree< T, _pb_, Allocator >::segment_tree ( InputIterator _first_, InputIterator _last_, allocator_type const & _a_,
                typename std::enable_if_t<  is_cpp17_input_iterator_v< InputIterator > &&
                                           !is_cpp17_forward_iterator_v< InputIterator > &&
                                            std::is_constructible_v<
                                                value_type,
                                                typename std::iterator_traits< InputIterator >::reference
                                        > > * )
        : _base( _a_ )
{
        capacity_      = NPL_RQ_DEFAULT_CAPACITY * 2;
        size_type size = NPL_RQ_DEFAULT_CAPACITY;

        _vallocate( capacity_ );
        _construct_at_end( size );

        for( ; _first_ != _last_; ++_first_ )
        {
                _emplace_back( *_first_ );
        }

        _rebuild_tree();
}

template< typename T, auto _pb_, typename Allocator >
template< typename ForwardIterator, typename U >
segment_tree< T, _pb_, Allocator >::segment_tree ( ForwardIterator _first_,
                typename std::enable_if_t< is_cpp17_forward_iterator_v< ForwardIterator > &&
                                           std::is_constructible_v<
                                                value_type,
                                                typename std::iterator_traits< ForwardIterator >::reference
                                        >,
                                        ForwardIterator > _last_ )
{
        size_type n = static_cast< size_type >( std::distance( _first_, _last_ ) );
        size_type size = _extend_to_pow2( n );
        capacity_ = size * 2;

        if( n > 0 )
        {
                _vallocate( capacity_ );
                _construct_at_end( size );
                _construct_at_end( _first_, _last_, n );
                _rebuild_tree();
        }
}

template< typename T, auto _pb_, typename Allocator >
template< typename ForwardIterator, typename U >
segment_tree< T, _pb_, Allocator >::segment_tree ( ForwardIterator _first_, ForwardIterator _last_, allocator_type const & _a_,
                typename std::enable_if_t< is_cpp17_forward_iterator_v< ForwardIterator > &&
                                           std::is_constructible_v<
                                                value_type,
                                                typename std::iterator_traits< ForwardIterator >::reference
                                        > > * )
        : _base( _a_ )
{
        size_type n = static_cast< size_type >( std::distance( _first_, _last_ ) );
        size_type size = _extend_to_pow2( n );
        capacity_ = size * 2;

        if( n > 0 )
        {
                _vallocate( capacity_ );
                _construct_at_end( size );
                _construct_at_end( _first_, _last_, n );
                _rebuild_tree();
        }
}

template< typename T, auto _pb_, typename Allocator >
segment_tree< T, _pb_, Allocator >::segment_tree ( segment_tree const & _x_ )
        : _base( _alloc_traits::select_on_container_copy_construction( _x_._alloc() ) )
{
        capacity_          = _x_.capacity_;
        size_type new_size = _x_.size();

        if( new_size > 0 )
        {
                _vallocate( capacity_ );
                _construct_at_end( capacity_ / 2 );
                _construct_at_end( _x_.begin(), _x_.end(), _x_.size() );
                _rebuild_tree();
        }
}

template< typename T, auto _pb_, typename Allocator >
segment_tree< T, _pb_, Allocator >::segment_tree ( segment_tree const & _x_, _identity< allocator_type > const & _a_ )
        : _base( _a_ )
{
        capacity_          = _x_.capacity_;
        size_type new_size = _x_.size();

        if( new_size > 0 )
        {
                _vallocate( capacity_ );
                _construct_at_end( capacity_ / 2 );
                _construct_at_end( _x_.begin(), _x_.end(), _x_.size() );
                _rebuild_tree();
        }
}

template< typename T, auto _pb_, typename Allocator >
inline
segment_tree< T, _pb_, Allocator >::segment_tree ( segment_tree && _x_ ) noexcept
        : _base( std::move( _x_._alloc() ) )
{
        this->capacity_  = _x_.capacity_;
        this->begin_     = _x_.begin_;
        this->end_       = _x_.end_;
        this->pb_        = _x_.pb_;
        this->_end_cap() = _x_._end_cap();

        _x_.begin_ = _x_.end_ = _x_._end_cap() = nullptr;
}

template< typename T, auto _pb_, typename Allocator >
inline
segment_tree< T, _pb_, Allocator >::segment_tree ( segment_tree && _x_, _identity< allocator_type > const & _a_ )
        : _base( _a_ )
{
        if( _a_ == _x_._alloc() )
        {
                this->capacity_  = _x_.capacity_;
                this->begin_     = _x_.begin_;
                this->end_       = _x_.end_;
                this->pb_        = _x_.pb_;
                this->_end_cap() = _x_._end_cap();

                _x_.begin_ = _x_.end_ = _x_._end_cap() = nullptr;
        }
        else
        {
                using _m_it = std::move_iterator< pointer >;
                assign( _m_it( _x_.begin_ ), _m_it( _x_.end_ ) );
        }
}

template< typename T, auto _pb_, typename Allocator >
inline
segment_tree< T, _pb_, Allocator >::segment_tree ( std::initializer_list< value_type > _list_ )
{
        if( _list_.size() > 0 )
        {
                auto size = _extend_to_pow2( _list_.size() );
                capacity_ = size * 2;

                _vallocate( capacity_ );
                _construct_at_end( size );
                _construct_at_end( _list_.begin(), _list_.end(), _list_.size() );
                _rebuild_tree();
        }
}

template< typename T, auto _pb_, typename Allocator >
inline
segment_tree< T, _pb_, Allocator >::segment_tree ( std::initializer_list< value_type > _list_, allocator_type const & _a_ )
        : _base( _a_ )
{
        if( _list_.size() > 0 )
        {
                auto size = _extend_to_pow2( _list_.size() );
                capacity_ = size * 2;

                _vallocate( capacity_ );
                _construct_at_end( size );
                _construct_at_end( _list_.begin(), _list_.end(), _list_.size() );
                _rebuild_tree();
        }
}

template< typename T, auto _pb_, typename Allocator >
inline
segment_tree< T, _pb_, Allocator > &
segment_tree< T, _pb_, Allocator >::operator= ( segment_tree && _x_ )
        noexcept(( noexcept_move_assign_container_v< Allocator, _alloc_traits > ))
{
        pb_       = _x_.pb_;
        capacity_ = _x_.capacity_;

        _move_assign( _x_, std::integral_constant< bool,
                        _alloc_traits::propagate_on_container_move_assignment::value >() );

        return *this;
}

template< typename T, auto _pb_, typename Allocator >
void
segment_tree< T, _pb_, Allocator >::_move_assign ( segment_tree & _c_, std::false_type )
        noexcept( _alloc_traits::is_always_equal::value )
{
        if( _base::_alloc() != _c_._alloc() )
        {
                using _m_it = std::move_iterator< pointer >;
                assign( _m_it( _c_.begin_ ), _m_it( _c_.end_ ) );
        }
        else
        {
                _move_assign( _c_, std::true_type() );
        }
}

template< typename T, auto _pb_, typename Allocator >
void
segment_tree< T, _pb_, Allocator >::_move_assign ( segment_tree & _c_, std::true_type )
        noexcept( std::is_nothrow_move_assignable_v< allocator_type > )
{
        _vdeallocate();
        _base::_move_assign_alloc( _c_ );

        this->capacity_  = _c_.capacity_;
        this->begin_     = _c_.begin_;
        this->end_       = _c_.end_;
        this->pb_        = _c_.pb_;
        this->_end_cap() = _c_._end_cap();

        _c_.begin_ = _c_.end_ = _c_._end_cap() = nullptr;
}

template< typename T, auto _pb_, typename Allocator >
inline
segment_tree< T, _pb_, Allocator > &
segment_tree< T, _pb_, Allocator >::operator= ( segment_tree const & _x_ )
{
        if( this != &_x_ )
        {
                _base::_copy_assign_alloc( _x_ );
                pb_ = _x_.pb_;
                capacity_ = _x_.capacity_;

                clear();

                _vallocate( capacity_ );
                _construct_at_end( capacity_ / 2 );
                _construct_at_end( _x_.begin(), _x_.end(), _x_.size() );
                _rebuild_tree();
        }
        return *this;
}

template< typename T, auto _pb_, typename Allocator >
template< typename InputIterator, typename U >
typename std::enable_if_t
<
         is_cpp17_input_iterator_v< InputIterator > &&
        !is_cpp17_forward_iterator_v< InputIterator > &&
         std::is_constructible_v
        <
                T,
                typename std::iterator_traits< InputIterator >::reference
        >,
        void
>
segment_tree< T, _pb_, Allocator >::assign ( InputIterator _first_, InputIterator _last_ )
{
        clear();

        for( ; _first_ != _last_; ++_first_ )
        {
                _emplace_back( *_first_ );
        }
}

template< typename T, auto _pb_, typename Allocator >
template< typename ForwardIterator, typename U >
typename std::enable_if_t
<
        is_cpp17_forward_iterator_v< ForwardIterator > &&
        std::is_constructible_v
        <
                T,
                typename std::iterator_traits< ForwardIterator >::reference
        >,
        void
>
segment_tree< T, _pb_, Allocator >::assign ( [[ maybe_unused ]] ForwardIterator _first_, [[ maybe_unused ]] ForwardIterator _last_ )
{

}

template< typename T, auto _pb_, typename Allocator >
void
segment_tree< T, _pb_, Allocator >::assign ( [[ maybe_unused ]] size_type _n_, [[ maybe_unused ]] const_reference _x_ )
{

}

template< typename T, auto _pb_, typename Allocator >
inline
typename segment_tree< T, _pb_, Allocator >::const_iterator
segment_tree< T, _pb_, Allocator >::_make_iter ( pointer _p_ ) const noexcept
{
        return iterator( _p_ );
}

template< typename T, auto _pb_, typename Allocator >
inline
typename segment_tree< T, _pb_, Allocator >::const_iterator
segment_tree< T, _pb_, Allocator >::begin () const noexcept
{
        return _make_iter( this->begin_ + capacity_ / 2 );
}

template< typename T, auto _pb_, typename Allocator >
inline
typename segment_tree< T, _pb_, Allocator >::const_iterator
segment_tree< T, _pb_, Allocator >::end () const noexcept
{
        return _make_iter( this->end_ );
}

template< typename T, auto _pb_, typename Allocator >
inline
typename segment_tree< T, _pb_, Allocator >::const_reference
segment_tree< T, _pb_, Allocator >::operator[] ( size_type _n_ ) const noexcept
{
        NPL_ASSERT( _n_ < size(), "segment_tree::[]: index out of bounds" );

        return this->begin_[ ( capacity_ / 2 ) + _n_ ];
}

template< typename T, auto _pb_, typename Allocator >
bool
segment_tree< T, _pb_, Allocator >::operator== ( segment_tree< T, _pb_, Allocator > const & _rhs_ ) const noexcept
{
        if( _capacity() != _rhs_._capacity() )
        {
                return false;
        }

        for( size_type i = 0; i < size(); ++i )
        {
                if( this->_at( i ) != _rhs_._at( i ) )
                {
                        return false;
                }
        }

        return true;
}

template< typename T, auto _pb_, typename Allocator >
auto
segment_tree< T, _pb_, Allocator >::operator+ ( segment_tree< T, _pb_, Allocator > const & _rhs_ ) const
{
        auto lsize = size();
        auto rsize = _rhs_.size();

        if( lsize > rsize )
        {
                segment_tree< T, _pb_, Allocator > res( begin(), end() );

                for( size_type i = 0; i < rsize; ++i )
                {
                        res._update( i, res.at( i ) + _rhs_.at( i ) );
                }

                return res;
        }
        else
        {
                segment_tree< T, _pb_, Allocator > res( _rhs_.begin(), _rhs_.end() );

                for( size_type i = 0; i < lsize; ++i )
                {
                        res._update( i, res.at( i ) + at( i ) );
                }

                return res;
        }
}

template< typename T, auto _pb_, typename Allocator >
auto &
segment_tree< T, _pb_, Allocator >::operator+= ( segment_tree< T, _pb_, Allocator > const & _rhs_ ) noexcept
{
        if( capacity_ == _rhs_.capacity_ )
        {
                for( size_type i = capacity_ / 2; i < capacity_; ++i )
                {
                        this->begin_[ i ] += _rhs_.begin_[ i ];
                }
        }
        else
        {
                if( capacity_ > _rhs_.capacity_ )
                {
                        for( size_type i = 0; i < _rhs_.capacity_ / 2; ++i )
                        {
                                this->begin_[ ( capacity_ / 2 ) + i ] += _rhs_.begin_[ ( _rhs_.capacity_ / 2 ) + i ];
                        }
                }
                else
                {
                        for( size_type i = 0; i < capacity_ / 2; ++i )
                        {
                                this->begin_[ ( capacity_ / 2 ) + i ] += _rhs_.begin_[ ( _rhs_.capacity_ / 2 ) + i ];
                        }
                        for( size_type i = capacity_ / 2; i < _rhs_.size(); ++i )
                        {
                                push_back( _rhs_.begin_[ ( _rhs_.capacity_ / 2 ) + i ] );
                        }
                }
        }

        return *this;
}

/*
template< typename T, auto _pb_, typename Allocator >
auto &
segment_tree< T, _pb_, Allocator >::operator+= ( segment_tree< T, _pb_, Allocator > const & _rhs_ ) noexcept
{
        if( size() >= _rhs_.size() )
        {
                for( size_type i = 0; i < _rhs_.size(); ++i )
                {
                        this->begin_[ ( this->capacity_ / 2 ) + i ] += _rhs_.begin_[ ( _rhs_.capacity_ / 2 ) + i ];
                }
        }
        else
        {
                for( size_type i = 0; i < size(); ++i )
                {
                        this->begin_[ ( capacity_ / 2 ) + i ] += _rhs_.begin_[ ( _rhs_.capacity_ / 2 ) + i ];
                }
                for( size_type i = size(); i < _rhs_.size(); ++i )
                {
                        push_back( _rhs_.begin_[ ( _rhs_.capacity_ / 2 ) + i ] );
                }
        }
        _rebuild_tree();

        return *this;
}
*/

template< typename T, auto _pb_, typename Allocator >
auto
segment_tree< T, _pb_, Allocator >::operator- ( segment_tree< T, _pb_, Allocator > const & _rhs_ ) const
{
        auto lsize = size();
        auto rsize = _rhs_.size();

        segment_tree< T, _pb_, Allocator > res( begin(), end() );

        if( lsize > rsize )
        {
                for( size_type i = 0; i < rsize; ++i )
                {
                        res._update( i, res.at( i ) - _rhs_.at( i ) );
                }
        }
        else
        {
                for( size_type i = 0; i < lsize; ++i )
                {
                        res._update( i, res.at( i ) - _rhs_.at( i ) );
                }
                for( size_type i = lsize; i < rsize; ++i )
                {
                        res.push_back( value_type() - _rhs_.at( i ) );
                }
        }

        return res;
}

template< typename T, auto _pb_, typename Allocator >
auto &
segment_tree< T, _pb_, Allocator >::operator-= ( segment_tree< T, _pb_, Allocator > const & _rhs_ ) noexcept
{
        if( size() > _rhs_.size() )
        {
                for( size_type i = 0; i < _rhs_.size(); ++i )
                {
                        this->begin_[ ( capacity_ / 2 ) + i ] -= _rhs_.begin_[ ( _rhs_.capacity_ / 2 ) + i ];
                }
        }
        else
        {
                for( size_type i = 0; i < size(); ++i )
                {
                        this->begin_[ ( capacity_ / 2 ) + i ] -= _rhs_.begin_[ ( _rhs_.capacity_ / 2 ) + i ];
                }
                for( size_type i = size(); i < _rhs_.size(); ++i )
                {
                        push_back( value_type() - _rhs_.begin_[ ( capacity_ / 2 ) + i ] );
                }
        }
        _rebuild_tree();

        return *this;
}

template< typename T, auto _pb_, typename Allocator >
typename segment_tree< T, _pb_, Allocator >::const_reference
segment_tree< T, _pb_, Allocator >::_at ( size_type _n_ ) const noexcept
{
        return this->begin_[ _n_ ];
}

template< typename T, auto _pb_, typename Allocator >
typename segment_tree< T, _pb_, Allocator >::const_reference
segment_tree< T, _pb_, Allocator >::at ( size_type _n_ ) const noexcept
{
        NPL_ASSERT( _n_ < size(), "segment_tree::at: index out of bounds" );

        return this->begin_[ ( capacity_ / 2 ) + _n_ ];
}

template< typename T, auto _pb_, typename Allocator >
typename segment_tree< T, _pb_, Allocator >::const_reference
segment_tree< T, _pb_, Allocator >::element_at ( size_type _n_ ) const noexcept
{
        NPL_ASSERT( _n_ < size(), "segment_tree::at: index out of bounds" );

        return this->begin_[ ( capacity_ / 2 ) + _n_ ];
}

template< typename T, auto _pb_, typename Allocator >
typename segment_tree< T, _pb_, Allocator >::value_type
segment_tree< T, _pb_, Allocator >::_range ( size_type _x_, size_type _y_ ) const noexcept
{
        _x_ += capacity_ / 2;
        _y_ += capacity_ / 2;

        value_type res {};
        bool is_res_default { true };

        while( _x_ <= _y_ )
        {
                if( _x_ % 2 == 1 )
                {
                        if( is_res_default )
                        {
                                res = this->begin_[ _x_++ ];
                                is_res_default = false;
                        }
                        else
                        {
                                res = this->pb_( res, this->begin_[ _x_++ ] );
                        }
                }
                if( _y_ % 2 == 0 )
                {
                        if( is_res_default )
                        {
                                res = this->begin_[ _y_-- ];
                                is_res_default = false;
                        }
                        else
                        {
                                res = this->pb_( res, this->begin_[ _y_-- ] );
                        }
                }
                _x_ /= 2;
                _y_ /= 2;
        }

        return res;
}

template< typename T, auto _pb_, typename Allocator >
typename segment_tree< T, _pb_, Allocator >::value_type
segment_tree< T, _pb_, Allocator >::range ( size_type _x_, size_type _y_ ) const noexcept
{
        NPL_ASSERT( _x_ < size() && _y_ < size(), "segment_tree::range: index out of bounds" );

        return _range( _x_, _y_ );
}

template< typename T, auto _pb_, typename Allocator >
typename segment_tree< T, _pb_, Allocator >::value_type
segment_tree< T, _pb_, Allocator >::range () const noexcept
{
        return size() ? _range( 0, size() - 1 ) : value_type();
}

template< typename T, auto _pb_, typename Allocator >
void
segment_tree< T, _pb_, Allocator >::update ( size_type _n_, const_reference _x_ )
{
        NPL_ASSERT( _n_ < size(), "segment_tree::update: index out of bounds" );

        _update( _n_, _x_ );
}

template< typename T, auto _pb_, typename Allocator >
void
segment_tree< T, _pb_, Allocator >::update ( size_type _n_, value_type && _x_ ) noexcept
{
        NPL_ASSERT( _n_ < size(), "segment_tree::update: index out of bounds" );

        _update( _n_, NPL_FWD( _x_ ) );
}

template< typename T, auto _pb_, typename Allocator >
template< typename... Args >
void
segment_tree< T, _pb_, Allocator >::update ( size_type _n_, Args... _args_ ) noexcept
{
        NPL_ASSERT( _n_ < size(), "segment_tree::update: index out of bounds" );

        _update( _n_, NPL_FWD( _args_ )... );
}

template< typename T, auto _pb_, typename Allocator >
void
segment_tree< T, _pb_, Allocator >::reserve ( [[ maybe_unused ]] size_type _n_ )
{

}

template< typename T, auto _pb_, typename Allocator >
void
segment_tree< T, _pb_, Allocator >::shrink_to_fit () noexcept
{

}

template< typename T, auto _pb_, typename Allocator >
template< typename U >
void
segment_tree< T, _pb_, Allocator >::_push_back_slow_path ( U && _x_ )
{
        allocator_type & a = this->_alloc();

        split_buffer< value_type, allocator_type & > b( capacity_ * 2, _size(), a );
        capacity_ *= 2;

        _alloc_traits::construct( a, std::to_address( b.end_ ), NPL_FWD( _x_ ) );

        b.end_++;
        _swap_out_circular_buffer( b );
}

template< typename T, auto _pb_, typename Allocator >
inline
void
segment_tree< T, _pb_, Allocator >::push_back ( const_reference _x_ )
{
        if( _size() < capacity_ )
        {
                _construct_one_at_end();
                _update( size() - 1, _x_ );
        }
        else
        {
                if( capacity_ * 2 <= _capacity() )
                {
                        _restructure_tree_forward();
                        _construct_one_at_end( _x_ );
                        _rebuild_tree();
                }
                else
                {
                        _push_back_slow_path( _x_ );
                        _restructure_tree_forward();
                        _rebuild_tree();
                        _update( size() - 1, _x_ );
                }
        }
}

template< typename T, auto _pb_, typename Allocator >
inline
void
segment_tree< T, _pb_, Allocator >::push_back ( value_type && _x_ )
{
        if( _size() < capacity_ )
        {
                _construct_one_at_end();
                _update( size() - 1, NPL_MOVE( _x_ ) );
        }
        else
        {
                if( capacity_ * 2 <= _capacity() )
                {
                        _restructure_tree_forward();
                        _construct_one_at_end( NPL_MOVE( _x_ ) );
                        _rebuild_tree();
                }
                else
                {
                        _push_back_slow_path( value_type() );
                        _restructure_tree_forward();
                        _rebuild_tree();
                        _update( size() - 1, NPL_MOVE( _x_ ) );
                }
        }
}

template< typename T, auto _pb_, typename Allocator >
template< typename... Args >
void
segment_tree< T, _pb_, Allocator >::_emplace_back_slow_path ( Args&&... _args_ )
{
        allocator_type & a = this->_alloc();

        split_buffer< value_type, allocator_type & > b( capacity_ * 2, _size(), a );
        capacity_ *= 2;

        _alloc_traits::construct( a, std::to_address( b.end_ ), NPL_FWD( _args_ )... );

        b.end_++;
        _swap_out_circular_buffer( b );
}

template< typename T, auto _pb_, typename Allocator >
template< typename... Args >
inline
typename segment_tree< T, _pb_, Allocator >::const_reference
segment_tree< T, _pb_, Allocator >::emplace_back ( Args&&... _args_ )
{
        if( _size() < capacity_ )
        {
                _construct_one_at_end();
                _update( size() - 1, NPL_FWD( _args_ )... );
        }
        else
        {
                if( capacity_ * 2 <= _capacity() )
                {
                        _restructure_tree_forward();
                        _construct_one_at_end( NPL_FWD( _args_ )... );
                        _rebuild_tree();
                }
                else
                {
                        _emplace_back_slow_path( NPL_FWD( _args_ )... );
                        _restructure_tree_forward();
                        _rebuild_tree();
                        _update( size() - 1, NPL_FWD( _args_ )... );
                }
        }

        return this->back();
}

template< typename T, auto _pb_, typename Allocator >
inline
void
segment_tree< T, _pb_, Allocator >::pop_back ()
{
        NPL_ASSERT( !empty(), "segment_tree::pop_back called on empty segment_tree" );

        *( --this->end_ ) = value_type(); // huehue
}

template< typename T, auto _pb_, typename Allocator >
void
segment_tree< T, _pb_, Allocator >::_move_range ( pointer _from_s_, pointer _from_e_, pointer _to_ )
{
        pointer old_last = this->end_;
        difference_type n = old_last - _to_;
        {
                pointer i = _from_s_ + n;
                _construct_transaction tx( *this, _from_e_ - i );

                for( pointer pos = tx.pos_; i < _from_e_;
                                ++i, ++pos, tx.pos_ = pos )
                {
                        _alloc_traits::construct( this->_alloc(), std::to_address( pos ), NPL_MOVE( *i ) );
                }
        }
        std::move_backward( _from_s_, _from_s_ + n, old_last );
}

template< typename T, auto _pb_, typename Allocator >
inline
void
segment_tree< T, _pb_, Allocator >::_invalidate_all_iterators ()
{

}

template< typename T, auto _pb_, typename Allocator >
inline
void
segment_tree< T, _pb_, Allocator >::_invalidate_iterators_past ( [[ maybe_unused ]] pointer _new_last_ )
{

}


} // namespace rq

} // namespace npl
