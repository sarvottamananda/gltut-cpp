cs3_assets = [ \
    'cubebox-0.png', \
    'cubebox-1.png', \
    'cubebox-2.png', \
    'cubebox-3.png', \
    'cubebox-4.png', \
    'cubebox-5.png', \
    'cubebox-6.png', \
    'cubebox-7.png', \
    'cubebox-8.png', \
    'cubebox-9.png', \
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
