# napi_render_glut
This is not full-featured GLUT node module. This only renders buffer as pixel array to fullscreen with GLUT
# install

    apt-get install -y freeglut3-dev

# usage example

    #!/usr/bin/env node
    mod = require("napi_render_glut");
    
    size_x  = 1920
    size_y  = 1080
    
    buffer = Buffer.alloc(3*size_x*size_y)
    for(var i=0;i<buffer.length;i+=3) {
      buffer[i+0] = 255;
      buffer[i+1] = 0;
      buffer[i+2] = 0;
    }
    
    mod.start_rgb(size_x, size_y, use_bgr=0)
    mod.draw_rgb(buffer)
    // NOTE fullscreen will be on next draw after window show animation is done
    setTimeout(()=>{
      mod.draw_rgb(buffer)
    }, 100);
    
    setTimeout(()=>{
      console.log("done");
    }, 10000);
