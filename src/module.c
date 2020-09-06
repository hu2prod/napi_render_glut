#include <node_api.h>

#include <GL/glut.h>
#include "type.h"


i32 set_size_x = 0;
i32 set_size_y = 0;

// u8* global_data = NULL;
GLenum px_format;

napi_value start_rgb(napi_env env, napi_callback_info info) {
  napi_status status;
  
  napi_value ret_dummy;
  status = napi_create_int32(env, 0, &ret_dummy);
  
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to create return value ret_dummy");
    return ret_dummy;
  }
  
  size_t argc = 3;
  napi_value argv[3];
  status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
  
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to parse arguments");
    return ret_dummy;
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  i32 size_x;
  status = napi_get_value_int32(env, argv[0], &size_x);
  
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Invalid i32 was passed as argument of size_x");
    return ret_dummy;
  }
  
  i32 size_y;
  status = napi_get_value_int32(env, argv[1], &size_y);
  
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Invalid i32 was passed as argument of size_y");
    return ret_dummy;
  }
  
  i32 use_bgr;
  status = napi_get_value_int32(env, argv[2], &use_bgr);
  
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Invalid i32 was passed as argument of use_bgr");
    return ret_dummy;
  }
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  if (use_bgr) {
    px_format = GL_BGR;
  } else {
    px_format = GL_RGB;
  }
  
  int skip_argc = 1;
  char *skip_argv[1] = {(char*)"no_arg"};
  glutInit(&skip_argc, skip_argv);
  glutInitDisplayMode(GLUT_SINGLE);  // Use single display buffer.
  glutInitWindowSize(size_x, size_y);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("GLUT render");
  // glutDisplayFunc(display);
  glutFullScreen();
  glutSetCursor(GLUT_CURSOR_NONE);
  
  // if (global_data) {
    // free(global_data);
  // }
  // global_data = (unsigned char*)malloc(3*size_x*size_y);
  
  set_size_x = size_x;
  set_size_y = size_y;
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  return ret_dummy;
}

napi_value draw_rgb(napi_env env, napi_callback_info info) {
  napi_status status;
  
  napi_value ret_dummy;
  status = napi_create_int32(env, 0, &ret_dummy);
  
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to create return value ret_dummy");
    return ret_dummy;
  }
  
  size_t argc = 1;
  napi_value argv[1];
  status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
  
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to parse arguments");
    return ret_dummy;
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  u8 *data_src;
  size_t data_src_len;
  status = napi_get_buffer_info(env, argv[0], (void**)&data_src, &data_src_len);
  
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Invalid buffer was passed as argument of data_src");
    return ret_dummy;
  }
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  if (data_src_len != (size_t)3*set_size_x*set_size_y) {
    napi_throw_error(env, NULL, "data_src_len != 3*set_size_x*set_size_y");
    return ret_dummy;
  }
  glRasterPos2i(-1, 1);
  glPixelZoom(1, -1);
  glClear(GL_COLOR_BUFFER_BIT);
  
  glDrawPixels(set_size_x, set_size_y, px_format, GL_UNSIGNED_BYTE, data_src);
  // glDrawPixels(set_size_x, set_size_y, px_format, GL_UNSIGNED_BYTE, global_data);
  
  glFlush();
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  
  return ret_dummy;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_value fn;
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  status = napi_create_function(env, NULL, 0, start_rgb, NULL, &fn);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to wrap native function");
  }
  
  status = napi_set_named_property(env, exports, "start_rgb", fn);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to populate exports");
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  status = napi_create_function(env, NULL, 0, draw_rgb, NULL, &fn);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to wrap native function");
  }
  
  status = napi_set_named_property(env, exports, "draw_rgb", fn);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to populate exports");
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)