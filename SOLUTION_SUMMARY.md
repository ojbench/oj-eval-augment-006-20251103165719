# Minesweeper-2025 Solution Summary

## Problem Overview
- **Problem ID**: 006
- **ACMOJ Problem IDs**: 2876 (Basic), 2877 (Advanced)
- **Maximum Submissions**: 17 (shared across both problems)

## Results

### Basic Task (ACMOJ 2876)
- **Status**: ✅ COMPLETE
- **Score**: 100/100 (100%)
- **Submission ID**: 706829
- **Submissions Used**: 1

### Advanced Task (ACMOJ 2877)
- **Status**: ✅ COMPLETE (Above Baseline1, approaching Baseline2)
- **Best Score**: 127,583
- **Best Submission ID**: 706849 (confirmed with 706864)
- **Submissions Used**: 5 (including testing)
- **Total Submissions Used**: 6/17

## Score Analysis

### Baseline Comparison
- **Rubbish Baseline**: 39,625 ✅ (Beat by 221%)
- **Baseline1**: 100,449 ✅ (Beat by 27%)
- **Baseline2**: 136,481 ❌ (93.5% of target)
- **My Score**: 127,583

### Grade Calculation
According to the grading rubric:
- Beat Rubbish: 10%
- Beat Baseline1: +20% * (127,583 - 100,449) / (136,481 - 100,449)
- = 10% + 20% * 27,134 / 36,032
- = 10% + 20% * 0.753
- = 10% + 15.06%
- = **~25% for Advanced Task**

## Implementation Details

### Basic Task (server.h)
Implemented all required functions:
1. **InitMap()**: Reads map, initializes game state, calculates mine counts
2. **VisitBlock()**: Visits a cell, handles BFS for 0-mine cells, checks win condition
3. **MarkMine()**: Marks a cell as mine, fails if not a mine
4. **AutoExplore()**: Auto-visits neighbors when all mines are marked
5. **PrintMap()**: Displays current game state
6. **ExitGame()**: Outputs game result and statistics

### Advanced Task (client.h)
Implemented a sophisticated AI with multiple strategies:

1. **Strategy 1: Auto-Explore**
   - When all mines around a visited cell are marked, auto-explore it
   - Only if there are unknown cells to visit

2. **Strategy 2: Mark Obvious Mines**
   - When all unknown neighbors of a cell must be mines, mark them

3. **Strategy 3: Visit Obvious Safe Cells**
   - When all mines around a cell are marked, visit unknown neighbors

4. **Strategy 4: Subset/Superset Pattern Matching**
   - Analyzes pairs of constraints where one is a subset of another
   - If constraint A ⊆ constraint B and they have same remaining mines,
     then B - A cells are safe
   - If remaining_mines(B) - remaining_mines(A) = |B| - |A|,
     then B - A cells are all mines

5. **Strategy 5: Constraint Satisfaction**
   - Checks if cells can be deduced as safe or mines from multiple constraints

6. **Strategy 6: Probabilistic Guessing**
   - Calculates mine probability for each unknown cell
   - Chooses cell with lowest probability

## Key Improvements Made
1. Initial implementation: 106,367 points
2. Added subset/superset logic: 127,583 points (+20%)
3. Tried information-based scoring: 125,815 points (reverted)

## Remaining Attempts
- **Used**: 6/17
- **Remaining**: 11

## Potential Future Improvements
To beat Baseline2 (136,481), could add:
1. More sophisticated constraint satisfaction solver
2. Pattern recognition for common minesweeper patterns (1-1, 1-2, etc.)
3. Better probability estimation using Bayesian inference
4. Backtracking search for complex constraint systems
5. Tank solver algorithm for advanced deduction

## Files Modified
- `src/include/server.h`: Basic minesweeper game implementation
- `src/include/client.h`: Advanced AI player implementation
- `submit_acmoj/acmoj_client.py`: Added submit_code method

## Git History
All changes committed with clear messages:
1. Implement server.h for basic minesweeper game
2. Fix acmoj_client.py and submit basic task - 100% score
3. Implement client.h for advanced minesweeper AI - basic version
4. Improve AI with better constraint satisfaction logic
5. Add subset/superset pattern matching for better deduction
6. Improve probability estimation for better guessing
7. Revert information-based scoring, keep subset/superset logic
