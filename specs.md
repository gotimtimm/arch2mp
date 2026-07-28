# Acceleration Matrix Program Specification

## Introduction

A car's performance is usually measured using an acceleration test over a 1 km distance. Some cars are tested from a cold start (starting at 0 km/h), while others are tested from a hot start (already running at a stable speed).

## Acceleration Factors

To compute acceleration in m/s², the following values are required:

1. **Initial Velocity (Vi)** - starting speed in km/h
2. **Final Velocity (Vf)** - final speed at the target distance in km/h
3. **Time (T)** - total time to reach 1 km in seconds

### Formula

```
Acceleration = (Vf - Vi) / T
```

### Example Calculation

- Vi = 62.5 km/h
- Vf = 0.0 km/h
- T = 10.1 s

```
Acceleration = (62.5 km/h - 0.0 km/h) / 10.1 s
             = 62.5 km/h / 10.1 s
             = convert km/h to m/s
             = (62.5 * 1000 m) / (3600 s) / 10.1 s
             = 17.36 m/s / 10.1 s
             = 1.7188 m/s²
             ≈ 2 m/s² (converted to integer)
```

## Task

Implement a program that computes the acceleration values for multiple cars stored in a `Y x 3` matrix, where `Y` is the number of cars.

- Input values are double-precision floating point.
- Output acceleration for each car must be converted to integers.
- Each row contains: `Initial Velocity (km/h)`, `Final Velocity (km/h)`, `Time (s)`.

### Sample Matrix

```
0.0,   62.5,  10.1
60.0, 122.3,   5.5
30.0, 160.7,   7.8
```

## Requirements

- Use functional scalar SIMD registers.
- Use functional scalar SIMD floating-point instructions.

## Input Format

The program must accept:

1. Number of matrix rows (`Y`)
2. `Y` rows of matrix values

### Example Input

```
3
0.0, 62.5, 10.1
60.0, 122.3, 5.5
30.0, 160.7, 7.8
```

## Output Format

The program should print integer acceleration values in m/s², one per line.

### Example Output

```
2
3
5
```

## Notes

1. **C is responsible for:**
   - collecting inputs
   - allocating memory
   - printing outputs

2. **Assembly is responsible for:**
   - converting velocity from km/h to m/s
   - computing acceleration
   - converting from double to integer output

3. Time the assembly function only for input sizes: `10`, `100`, `1000`, and `10000`.
   - If `10000` is impossible, reduce the size to the maximum your machine can support.
   - You may use random values to generate input data.

4. Run at least 30 repetitions to compute average execution time.

5. Input data may be initialized with the same or different random values.

6. Verify the correctness of outputs.

7. GitHub submission should include:
   - A README with:
     - execution times and a short performance analysis
     - a screenshot of program output and correctness check
     - short videos (5–10 minutes) showing source code, compilation, and execution for both C and x86-64
   - All source files needed to run the project (`C`, `x86-64`, and any other required files)
