#!/bin/bash

# Script para crear videos a partir de los frames generados por la versión secuencial

# Funcion para crear video a partir de los frames secuenciales 
create_sequential_video() {
    echo "Creating video from sequential frames..."
    ffmpeg -framerate 24 -i frame_%d.png -c:v libx264 -pix_fmt yuv420p crossfade_sequential.mp4
}

main() {
    if [ -f "frame_0.png" ]; then
        create_sequential_video
    fi
    echo "Video creation complete."
    echo "Generated videos:"
    ls -la *.mp4
}

main