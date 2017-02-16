# Rtesseract package

This is an R interface to the tesseract OCR (Optical Character Recognition) system.

tesseract is available at https://code.google.com/p/tesseract-ocr/.

More recent versions are available on github
  https://github.com/tesseract-ocr/tesseract

Installing tesseract involves first installing leptonica
http://www.leptonica.com/.

This is currently a basic interface to the essential functionality, with some
added R functionality to visualize the results.

+ Of course, the package provides functionality to get the recognized text.
However, it also allows us to do this at various different levels, e.g.
word, character, line
+ We can also use different page segmentation modes so that we can detect/recognize
lines on the image which is useful for processing tables where the lines separate
rows or columns
+ We can get the confidence for each recognized text element to understand whether it is 
  a good match or not.
+ We can get the location and dimensions of each of the text elements. Again, this is 
 necessary for processing tables and other structured content.
+ We can display the matched text, the associated confidences to see spatial patterns.
 Also, we can overlay this on the original image to see patterns.
+ We can restrict the recognition to a sub-rectangle of the image.
+ The package provides lower-level access to the C++ API, allowing for more fine-grained and efficient
 use and flexible programmatic access.
+ We can set and query many variables cotrolling tesseract's behaviour.
+ We can query details about the image.
+ We can query the metadata about the version of tesseract, the supported image formats, etc.


We can machine generate the interface to the other methods and classes in the tesseract API/library.


## History
We started developing this package in April 2015.