cs3_assets = [ \
    'skybox.vert', \
    'skybox.frag', \
    'cubeobj.vert', \
    'cubeobj.frag', \
] 

cs3_asset_targets = []
foreach asset_file : cs3_assets
    cs3_asset_targets += custom_target(
        'asset - @0@'.format(asset_file),
        input : asset_file, 
        output : asset_file,
        command : ['cp', '@INPUT@', '@OUTPUT@'],
        install : true,
        install_dir : '.'
    )
endforeach
