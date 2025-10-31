# Pathfinding Example


## Set up

```
mkdir build
cd build
```

**Configure**

Debug build:

```
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

Release build:

```
cmake -DCMAKE_BUILD_TYPE=Release ..
```

**Build**

```
cmake --build . --parallel
```

**Run**

```
./pathfinding-example
```

## Stats

**seed = 1, grid_size = 20**

|  | Nodes Explored | Path length | Path Cost |
| :--: |  :------------- | :-- | :-- |
| DFS | 309 | 119 | 3766 |
| BFS | 368 | 21 | 564 |
| UCS |  |  |   |
| A*  |   |  |  |

**seed = 1, grid_size = 40**

| | Nodes Explored | Path length | Path Cost |
| :--: |  :------------- | :-- | :-- |
| DFS | 1409 | 629 | 9966 |
| BFS | 1568 | 61 | 589 |
| UCS |  |  |   |
| A*  |   |  |  |
