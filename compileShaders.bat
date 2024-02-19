glslangValidator -V -gVS geometry.vert -o geometry.vert.spirv
glslangValidator -V -gVS geometry.frag -o geometry.frag.spirv

glslangValidator -V -gVS swapchain.vert -o swapchain.vert.spirv
glslangValidator -V -gVS swapchain.frag -o swapchain.frag.spirv

glslangValidator -V -gVS waveform.comp -o waveform.comp.spirv
glslangValidator -V -gVS waveformMax.comp -o waveformMax.comp.spirv
glslangValidator -V -gVS waveformMipLevel.comp -o waveformMipLevel.comp.spirv
glslangValidator -V -gVS waveform.vert -o waveform.vert.spirv
glslangValidator -V -gVS waveform.frag -o waveform.frag.spirv


PAUSE