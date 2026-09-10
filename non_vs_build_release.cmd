mkdir out
cd out
cmake ..
msbuild Game2d.sln /p:Configuration=Release /p:Platform=x64

cd Release/bin
move Release\Game2d.exe .
move Release\libpng16-16.dll .
move Release\libwebp.dll .
move Release\libwebpdemux.dll .
move Release\libwebpmux.dll .
move Release\SDL3.dll .
move Release\SDL3_image.dll .
move Release\SDL3_ttf.dll .
move Release\tiff.dll.dll .
