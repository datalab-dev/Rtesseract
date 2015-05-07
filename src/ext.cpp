#include <tesseract/baseapi.h>
#include <Rdefines.h>

#include <leptonica/allheaders.h>

void R_pixDestroy(SEXP obj);


#define GET_REF(obj, type) \
  (type *) R_ExternalPtrAddr(GET_SLOT(obj, Rf_install("ref")))

SEXP createRef(void *ptr, const char * const classname, R_CFinalizer_t fin);
void R_freeAPI(SEXP obj);
void R_freeResultIterator(SEXP obj);

extern "C"
SEXP
R_TessBaseAPI_new()
{
  tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
  return(createRef(api, "TessBaseAPI", R_freeAPI));
}

void
R_freeAPI(SEXP obj)
{
  tesseract::TessBaseAPI * api = (tesseract::TessBaseAPI *)  R_ExternalPtrAddr(obj)  ;
  if(api) {
#ifdef FINALIZER_DEBUG
    Rprintf("R_freeAPI\n");
#endif
    delete api;
  }
}

extern "C"
SEXP
R_TessBaseAPI_Init(SEXP r_api, SEXP r_lang)
{
  tesseract::TessBaseAPI * api = GET_REF(r_api, tesseract::TessBaseAPI );
  if(!api) {
      PROBLEM "NULL value for api reference"
      ERROR;
  }

  const char *lang = CHAR(STRING_ELT(r_lang, 0));
  int ok = api->Init(NULL, lang); 
  return( ScalarLogical( ok == 0 ));
}

extern "C"
SEXP
R_TessBaseAPI_SetVariables(SEXP r_api, SEXP r_vars)
{
  tesseract::TessBaseAPI * api = GET_REF(r_api, tesseract::TessBaseAPI);
  if(!api) {
      PROBLEM "NULL value for api reference"
      ERROR;
  }

  SEXP r_optNames = GET_NAMES(r_vars);
  int i;
  for(i = 0; i < Rf_length(r_vars); i++) 
      api->SetVariable(CHAR(STRING_ELT(r_optNames, i)), CHAR(STRING_ELT(r_vars, i)));

  return( ScalarInteger( i ));
}


extern "C"
SEXP
R_TessBaseAPI_SetImage(SEXP r_api, SEXP r_img)
{
  tesseract::TessBaseAPI * api = GET_REF(r_api, tesseract::TessBaseAPI);
  if(!api) {
      PROBLEM "NULL value for api reference"
      ERROR;
  }

  Pix *img = GET_REF(r_img, Pix);
  if(!img) {
      PROBLEM "NULL value passed for image (Pix)"
      ERROR;
  }
  api->SetImage(img);

  return(R_NilValue);
}

extern "C"
SEXP
R_pixRead(SEXP r_filename)
{
  Pix *image = pixRead(CHAR(STRING_ELT(r_filename, 0)));
  return(createRef(image, "Pix", R_pixDestroy));
}

void
R_pixDestroy(SEXP obj)
{
  Pix *p = (Pix *) R_ExternalPtrAddr(obj);
  if(p) {
#ifdef FINALIZER_DEBUG
     Rprintf("R_pixDestroy\n");
#endif
     pixDestroy(&p);
     R_SetExternalPtrAddr(obj, NULL);
  }
}

SEXP
createRef(void *ptr, const char * const classname, R_CFinalizer_t fin)
{
  SEXP robj, klass, ref;

 
  PROTECT(klass = MAKE_CLASS(classname));
  PROTECT(robj = NEW(klass));
  SET_SLOT(robj, Rf_install("ref"), ref = R_MakeExternalPtr(ptr, Rf_install(classname), R_NilValue));

  // Set finalizer to garbage collect when we let go/release this object.
  if(fin)
     R_RegisterCFinalizer(ref, fin);
  UNPROTECT(2);  
  return(robj);
}


extern "C"
SEXP
R_TessBaseAPI_Recognize(SEXP r_api)
{
  tesseract::TessBaseAPI * api = GET_REF(r_api, tesseract::TessBaseAPI);
  if(!api) {
      PROBLEM "NULL value for api reference"
      ERROR;
  }
  return(ScalarLogical(api->Recognize(NULL) == 0));
}


extern "C"
SEXP
R_TessBaseAPI_GetIterator(SEXP r_api)
{
  tesseract::TessBaseAPI * api = GET_REF(r_api, tesseract::TessBaseAPI);
  if(!api) {
      PROBLEM "NULL value for api reference"
      ERROR;
  }
  tesseract::ResultIterator* ri = api->GetIterator();
  return(createRef(ri, "ResultIterator", R_freeResultIterator));
}


typedef SEXP (*NativeIteratorFun)(tesseract::ResultIterator *, tesseract::PageIteratorLevel);

extern "C"
SEXP
R_ResultIterator_lapply(SEXP r_it, SEXP r_level, SEXP r_fun)
{
   tesseract::ResultIterator *ri = GET_REF(r_it, tesseract::ResultIterator);
   ri->Begin();
   int n = 1, i;

   tesseract::PageIteratorLevel level = (tesseract::PageIteratorLevel) INTEGER(r_level)[0];

   while(ri->Next(level)) n++;

   SEXP names, ans, el;
   PROTECT(names = NEW_CHARACTER(n));
   PROTECT(ans = NEW_LIST(n));   
   i = 0;
   ri->Begin();

   NativeIteratorFun fun = NULL;

   if(TYPEOF(r_fun) == EXTPTRSXP)
     fun = (NativeIteratorFun) R_ExternalPtrAddr(r_fun);

   do {
      const char* word = ri->GetUTF8Text(level);
      SET_STRING_ELT(names, i, Rf_mkChar(word));
      if(fun)
	el = fun(ri, level);
      else
        el = Rf_eval(r_fun, R_GlobalEnv);
      SET_VECTOR_ELT(ans, i, el);
      delete[] word;
      i++;
   } while(ri->Next(level));

   SET_NAMES(ans, names);
   UNPROTECT(2);
   return(ans);
}

/* eventhough we don't call this directly from R, we need to be able to get the symbol.
 */
extern "C"  
SEXP
r_getConfidence(tesseract::ResultIterator *ri, tesseract::PageIteratorLevel level)
{
  return(ScalarReal( ri->Confidence(level) ));
}

void
R_freeResultIterator(SEXP obj)
{
  tesseract::ResultIterator* api = (tesseract::ResultIterator *)  R_ExternalPtrAddr(obj)  ;
  if(api)
    delete api;
}




extern "C"
SEXP
R_ResultIterator_BoundingBox(SEXP r_it, SEXP r_level)
{

   tesseract::ResultIterator *ri = GET_REF(r_it, tesseract::ResultIterator);
   tesseract::PageIteratorLevel level = (tesseract::PageIteratorLevel) INTEGER(r_level)[0];

    int x1, y1, x2, y2;
    ri->BoundingBox(level, &x1, &y1, &x2, &y2); 
    SEXP tmp = NEW_NUMERIC(4); // Note: Don't need to protect.
      REAL(tmp)[0] = x1;
      REAL(tmp)[1] = y1;
      REAL(tmp)[2] = x2;
      REAL(tmp)[3] = y2;
    return(tmp);
}


extern "C"
SEXP
R_ResultIterator_Confidence(SEXP r_it, SEXP r_level)
{

   tesseract::ResultIterator *ri = GET_REF(r_it, tesseract::ResultIterator);
   tesseract::PageIteratorLevel level = (tesseract::PageIteratorLevel) INTEGER(r_level)[0];

   return(ScalarReal(ri->Confidence(level)));
}

extern "C"
SEXP
R_ResultIterator_GetUTF8Text(SEXP r_it, SEXP r_level)
{

   tesseract::ResultIterator *ri = GET_REF(r_it, tesseract::ResultIterator);
   tesseract::PageIteratorLevel level = (tesseract::PageIteratorLevel) INTEGER(r_level)[0];

   const char * val = ri->GetUTF8Text(level);
   SEXP ans = ScalarString(mkChar(val));
   delete[] val;

   return(ans);
}



extern "C"
SEXP
R_tesseract_GetInitLanguagesAsString(SEXP r_api)
{
  tesseract::TessBaseAPI * api = GET_REF(r_api, tesseract::TessBaseAPI);
  return(ScalarString(mkChar(api->GetInitLanguagesAsString())));
}



extern "C"
SEXP
R_tesseract_ReadConfigFile(SEXP r_api, SEXP r_filename)
{
  tesseract::TessBaseAPI * api = GET_REF(r_api, tesseract::TessBaseAPI );
  for(int i = 0; i < Rf_length(r_filename); i++)
    api->ReadConfigFile(CHAR(STRING_ELT(r_filename, i)));

  return(R_NilValue);
}


extern "C"
SEXP
R_tesseract_SetSourceResolution(SEXP r_api, SEXP r_ppi)
{
  tesseract::TessBaseAPI * api = GET_REF(r_api, tesseract::TessBaseAPI );
  api->SetSourceResolution(INTEGER(r_ppi)[0]);

  return(R_NilValue);
}


extern "C"
SEXP
R_tesseract_SetRectangle(SEXP r_api, SEXP r_dims)
{
  tesseract::TessBaseAPI * api = GET_REF(r_api, tesseract::TessBaseAPI );
  int *d = INTEGER(r_dims);
  api->SetRectangle(d[0], d[1], d[2], d[3]);

  return(R_NilValue);
}



extern "C"
SEXP
R_tesseract_Clear(SEXP r_api)
{
  tesseract::TessBaseAPI * api = GET_REF(r_api, tesseract::TessBaseAPI );
  api->Clear();

  return(R_NilValue);
}


