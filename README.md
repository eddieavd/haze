# haze

**haze** is a C++ image manipulation library utilizing both CPU- and GPU-side processing to provide high performance image transformations, 
either through traditional kernel convolution approaches or with specialized implementations using custom data structures.  
  
## general

One of the cornerstones of the library is the image class, a class templated on the pixel type and used for storing images.  
However, besides acting as an image container, the class also provides some utilities such as constructing a subimage from an existing image.    

Making things a bit more useful, the library introduces the pixel field, a slightly smarter (and fatter) image container.  
The pixel field, instead of storing a simple buffer of 8 bit pixel values, stores the pixels in two-dimensional prefix arrays. This enables the container
to calculate sums of pixel values in any subimage in constant time.  
With this approach, it's possible to perform a mean blur with any blur radius on an _n x m_ image in _O( n * m )_ time, 
while a naive approach would take _O( n * m * blur_radius * blur_radius )_ (or _O( n * m * blur_radius * 2 )_ when using separable convolutions).

### examples
#### converting image formats
```C++
#include "include/HAZElib.hpp"

int main ()
{
    haze::image< haze::rgb_pixel > img( "image.jpeg" );
    
    haze::util::write_to_png( img, "image.png" );
    
    
    return 0;
}
```
#### scaling images
```C++
{
    haze::image< haze::rgb_pixel > img( "image.png" );
    
    auto scaled_up  = haze::scale_image( img, 2.0, 2.0, haze::scaling_options::interpolation_type::bilinear         );
    auto scaled_dwn = haze::scale_image( img, 0.5, 0.5, haze::scaling_options::interpolation_type::nearest_neighbor );
}
```
#### getting and setting subimages
```C++
{
    haze::image< haze::rgb_pixel > img(   "image.png" );
    haze::image< haze::rgb_pixel > sml( "smaller.png" );
    
    auto sub = img.get_subimage( 0, 0, 400, 300 );
    
    img.set_subimage( sml, 0, 0 );
}
```
#### CPU-side mean blur with pixel field
```C++
constexpr int blur_radius = 32;

{
    haze::pixel_field< haze::rgb_pixel > field( "image.png" );
    
    haze::image< haze::rgb_pixel > blr = field.get_blurred_image( blur_radius );
}
{
    haze::image< haze::rgb_pixel > img( "image.png" );
    
    haze::pixel_field field( img );
    
    auto blr = field.get_blurred_image( blur_radius );
}
```

## img_transform

The **img_transform** header provides utilities necessary for traditional kernel convolutions including 2D square and rectangular kernels as well as separable 1D kernels. 
It also exposes several free functions which implement the image transformations CPU-side, but they're only meant to be used as benchmarks against the GPU implementations.

### examples
#### edge detection with sobel operator
```C++
{
    haze::image< haze::bw_pixel > img( "image.png" );
    
    auto v_edge = haze::transform_image( img, haze::sobel_v );
    auto h_edge = haze::transform_image( img, haze::sobel_h );
}
```

## perlin noise

1d 2d 3d perlin noise generator

## steganography

basic af, stores a message in the lower 2 bits of a pixel

## CPU side
cpu does stuff.  

## GPU side
gpu goes brrrr.  

### examples
#### kernel convolutions
```C++
{
    haze::gpu_ops< haze::rgb_pixel > gpu;
    
    haze::image< haze::rgb_pixel > img( "image.png" );
    
    auto gauss_3 = gpu.transform_image(     img, haze::gauss_3 );
    auto v_edge  = gpu.transform_image( gauss_3, haze::sobel_v );
    auto h_edge  = gpu.transform_image( gauss_3, haze::sobel_h );
}
```
#### full edge detection
```C++
{
    haze::gpu_ops< haze::bw_pixel > gpu;
    
    haze::image< haze::bw_pixel > img( "image.png" );
    
    auto blurred   = gpu.transform_image( img, haze::gauss_3 );
    auto full_edge = gpu.detect_edges( img );
}
```
