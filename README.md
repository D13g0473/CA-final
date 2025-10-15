# Cross-Fading Image Processing - Parallel Computing Project

This project implements cross-fading between a color image and its grayscale version using sequential, MPI (distributed memory), and OpenMP (shared memory) approaches.

## Project Structure

- `sequential.cpp` - Sequential implementation
- `mpi_parallel.cpp` - MPI parallel implementation for distributed memory
- `openmp_parallel.cpp` - OpenMP parallel implementation for shared memory
- `benchmark.sh` - Script to run performance benchmarks
- `analyze_results.py` - Python script to analyze results and generate reports
- `create_video.sh` - Script to create videos from generated frames

## Requirements

- C++ compiler with C++11 support
- OpenCV 4.x
- MPI library (e.g., OpenMPI)
- OpenMP support
- Python 3 with pandas, matplotlib, numpy
- ImageMagick (for creating test images)
- FFmpeg (for video creation)

## Compilation

```bash
# Sequential version
g++ -std=c++11 sequential.cpp -o sequential `pkg-config --cflags --libs opencv4`

# MPI version
mpic++ -std=c++11 mpi_parallel.cpp -o mpi_parallel `pkg-config --cflags --libs opencv4`

# OpenMP version
g++ -std=c++11 -fopenmp openmp_parallel.cpp -o openmp_parallel `pkg-config --cflags --libs opencv4`
```

## Usage

### Running Benchmarks

```bash
chmod +x benchmark.sh
./benchmark.sh
```

This will:
1. Create test images of sizes 800x800, 2000x2000, and 5000x5000
2. Compile all programs
3. Run benchmarks for each implementation and configuration
4. Generate results.csv with timing data

### Analyzing Results

```bash
python3 analyze_results.py
```

This will generate:
- `results_with_metrics.csv` - Results with speedup and efficiency
- `performance_analysis.png` - Performance plots
- `speedup_efficiency_analysis.png` - Speedup and efficiency plots
- `performance_report.md` - Detailed analysis report

### Creating Videos

```bash
chmod +x create_video.sh
./create_video.sh
```

## Problem Description

The program generates a 4-second video (96 frames at 24 fps) showing the cross-fading transition from a color image to its grayscale version. The cross-fading formula used is:

```
result[x][y].r = color[x][y].r * P + gray[x][y].r * (1-P)
result[x][y].g = color[x][y].g * P + gray[x][y].g * (1-P)
result[x][y].b = color[x][y].b * P + gray[x][y].b * (1-P)
```

Where P varies from 1.0 to 0.0 over the 96 frames.

## Parallel Implementations

### MPI (Distributed Memory)
- Uses message passing for communication between processes
- Distributes image rows among processes for parallel processing
- Suitable for clusters and multi-node systems

### OpenMP (Shared Memory)
- Uses threads for parallel processing within a single node
- Parallelizes loops using `#pragma omp parallel for`
- Suitable for multi-core systems

## Performance Metrics

- **Speedup**: Sequential time / Parallel time
- **Efficiency**: Speedup / Number of processes/threads

## Results

The analysis compares performance across different image sizes (800x800, 2000x2000, 5000x5000) and parallel configurations (2, 4, 8 processes/threads).

## Video Creation

Generated frames can be combined into videos using FFmpeg. The resulting videos demonstrate the smooth cross-fading effect between color and grayscale versions of the input image.

## Academic Context

This project fulfills the requirements of the "Computación Avanzada" course final practical assignment, demonstrating understanding of parallel programming concepts, performance analysis, and comparative implementation of sequential vs. parallel algorithms.# CA-final
