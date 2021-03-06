\name{IFF_PNG}
\alias{pixWrite}
\alias{IFF_UNKNOWN}
\alias{IFF_BMP}
\alias{IFF_JFIF_JPEG}
\alias{IFF_PNG}
\alias{IFF_TIFF}
\alias{IFF_TIFF_PACKBITS}
\alias{IFF_TIFF_RLE}
\alias{IFF_TIFF_G3}
\alias{IFF_TIFF_G4}
\alias{IFF_TIFF_LZW}
\alias{IFF_TIFF_ZIP}
\alias{IFF_PNM}
\alias{IFF_PS}
\alias{IFF_GIF}
\alias{IFF_JP2}
\alias{IFF_WEBP}
\alias{IFF_LPDF}
\alias{IFF_DEFAULT}
\alias{IFF_SPIX}

\title{File Format Symbolic Constants}
\description{
  \code{pixWrite} creates a file containing the image in the \code{Pix}.
  The code uses the leptonica library and, depending on how it was
  installed, it has support for numerous image formats.


The IFF (ImageFileFormat) variables are symbolic constants used to identify different image
file formats. We use them in \code{\link{pixWrite}} to indicate the
desired file format for the image being written.
These are better than strings, e.g. "PNG", or simple numbers, e.g. 3.
However, one can use either of these forms also. However, \code{IFF_PNG}
is preferred to "PNG" and both are  much preferred to 3.
}
\usage{
pixWrite(pix, file, format = guessImageFormatByExt(file)) 
}
\arguments{
  \item{pix}{the \code{Pix} object containing the imagex}
  \item{file}{the name of the file to which the image will be written.}
  \item{format}{the file format to use. By default, we attempt to guess
    the format from the extension on the \code{file} name. However, one
    can specify it using the \code{IFF_} variables, e.g. \code{IFF_PNG},
    or by a string corresponding to the suffix of one of these IFF
    varibles,
    e.g. \code{"PNG"} or \code{"png"}.
    We discourage using the integer values of these \code{IFF_}
    variables.
  The code is less readable and these values may change in the future,
  but not the symbolic names.}
}
\source{
Leptonica imageio.h heade file.
}
\references{
\url{http://www.leptonica.com}
}
\seealso{
  \code{\link{pixWrite}}
}
\examples{
\dontrun{
  f = system.file("images", "DifferentFonts.png", package = "Rtesseract")
  p = pixRead(f)
  pixWrite(p, "foo.webp", IFF_WEBP)
  pixWrite(p, "foo.tiff", IFF_TIFF_LZW)
  pixWrite(p, "foo.webp", "WEBP")
  pixWrite(p, "foo.tiff", "TIFF_LZW")

  pixWrite(p, "foo.webp")
}
}
\keyword{datasets}
