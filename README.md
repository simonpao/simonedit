# Simon Edit

Command line text editor build using C. Reads every line of a file into a doubly linked list for in memory editing. Saves to file as newline (`\n`) separated lines of text. (`P`)rint, (`G`)oto, (`C`)ount, (`M`)ove, (`D`)elete, and (`I`)nsert commands are available for editing. 

## Build

Compile all source code using Makefile.

```bash
$ make
$ ./simonedit.out
```

## Usage

File extension must be *.simon. To read a file named `text.simon` in a directory `text`, enter the following:

```bash
$ ./simonedit.out 
Enter the name of the file to edit: text/text
5 lines read.
cmd:  P^,$
001:  line 1
002:  line 2
003:  line 3
004:  line 4
005:  line 5
```

or simply:

```bash
$ ./simonedit.out text/text.simon
```

### Editing commands

Available commands are:

- `P` - Print line
- `G` - Change the active line (goto)
- `D` - Delete line
- `I` - Insert lines, newline to end
- `M` - Move line
- `C` - Count lines
- `W` - Save changes
- `Q` - Quit (Will be prompted to save if there are changes)

#### Print line(s)

```bash
cmd:  P      # Prints all lines of the file (same as P^,$)
cmd:  P1     # Prints line 1
cmd:  P1,10  # Prints lines 1 through 10
cmd:  P1,$   # Prints lines from 1 to end of file
cmd:  P^,10  # Prints lines from beginning to line 10
cmd:  P.,10  # Prints from active line to line 10
cmd:  P.+1   # Print line after active line
cmd:  P.-1   # Print line before active line
```

#### Goto line

```bash
cmd:  G1     # Go to line 1
cmd:  G$     # Go to last line
cmd:  G^     # Go to first line
cmd:  G.+1   # Go to line after active line
cmd:  G.-1   # Go to line before active line
```

#### Move line

```bash
cmd:  M1     # Move line 1
cmd:  M1,5   # Move lines 1 through 5
cmd:  M^,5   # Move first through fifth lines
cmd:  M5,$   # Move fifth through last lines
```

#### Delete line

```bash
cmd:  D1     # Delete line 1
cmd:  D1,5   # Delete lines 1 through 5
cmd:  D^,5   # Delete first through fifth lines
cmd:  D5,$   # Delete fifth through last lines
cmd:  D.     # Delete active line
cmd:  D.+1   # Delete line after active line
cmd:  D^,$   # Delete all lines
```

#### Insert line

```bash
cmd:  I1     # Insert before line 1
cmd:  I      # Insert before active line
```

#### Count lines

```bash
cmd:  C      # Count the number of lines in the file (same as C^,$)
cmd:  C.,$   # Count the number of lines from the current line to the last line (inclusive)
cmd:  C1     # Count the number of characters on line 1
```

#### Save changes

```bash
cmd:  W      # Save changes to current file
cmd:  Wtext2 # Save changes to a file named 'text2.simon'
```

#### Quit

```bash
cmd:  Q      # Close file without saving13, cmd: q
File modified. Enter W to save, Q to discard.
cmd:  Q
```


## Known Issues

List of known issues to be addressed

### General

- ~~The minus (-) operator does not work for selecting relative lines~~

### Printing lines

- ~~Printing backwards shows same data for all lines printed~~

### Moving lines

- Moving lines when first line is active deletes the moved line(s) and does not reset line numbers
    ```bash
    1, cmd: m3
    2, cmd: p^,$
    002:  line 1
    003:  line 2
    004:  line 4
    005:  line 5
    5, cmd:
    ```

### Deleting lines

- ~~Deleting all lines and then printing results in segmentation fault~~
- ~~Deleting last line (which loops active line to line 1) and then printing from current line results in segmentation fault~~
- Deleting all lines does not change the active line
    ```bash
    5, cmd: d^,$
    5, cmd: p
    File is empty.
    5, cmd: 
    ```
- ~~Deleting the last line remaining in the file results in error~~
- Deleting last line then printing current line does not print the line content
    ```bash
    5, cmd: d.
    1, cmd: p.
    001:  1, cmd:
    ```

### Inserting lines

- ~~Cannot insert lines after the last line - should be able to specify line number greater than the last line~~