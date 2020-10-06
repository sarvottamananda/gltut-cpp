cs3_assets = [ \
    'textures/skybox-front.jpg', \
    'textures/skybox-back.jpg', \
    'textures/skybox-top.jpg', \
    'textures/skybox-bottom.jpg', \
    'textures/skybox-left.jpg', \
    'textures/skybox-right.jpg', \
    'textures/cc-textures.jpg', \
    \
    'shaders/cubeobj.frag', \
    'shaders/cubeobj.vert', \
    'shaders/skybox.frag', \
    'shaders/skybox.vert', \
    'shaders/ground.vert', \
    'shaders/ground.frag', \
] 

foreach asset_file : cs3_assets
  run_command(
    copy_to_build,
    asset_file
  )
endforeach
