# haze <br/>

**haze** is a C++ image manipulation library utilizing both CPU- and GPU-side processing to provide high performance image transformations,  
either through traditional kernel convolution approaches or with specialized implementations using custom data structures.  
  
## general <br/>

One of the cornerstones of the library is the image class, a class templated on the pixel type and used for storing images.  
However, besides acting as an image container, the class also provides some utilities such as constructing a subimage from an existing image.    

Making things a bit more useful, the library introduces the pixel field, a slightly smarter (and fatter) image container.  
The pixel field, instead of storing a simple buffer of 8 bit pixel values, stores the pixels in two-dimensional prefix arrays. This enables the container
to calculate sums of pixel values in any subimage in constant time.  
With this approach, it's possible to perform a mean blur with any blur radius on an _n x m_ image in _O( n * m )_ time, 
while a naive approach would take _O( n * m * blur_radius * blur_radius )_ (or _O( n * m * blur_radius * 2 )_ when using separable convolutions).

## img_transform <br/>

traditional kernel convolutions n shiet

## perlin noise <br/>

1d 2d 3d perlin noise generator

## steganography <br/>

basic af, stores a message in the lower 2 bits of a pixel

## CPU side <br/>
cpu does stuff.  

## GPU side <br/>
gpu goes brrrr.  
