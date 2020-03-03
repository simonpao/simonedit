# Simon Edit

Command line text editor.

## Build

Compile all source code using Makefile.

```bash
$ make
$ ./simonedit.out
```

## Usage

File extension must be *.simon. To read a file named text.simon in a directory text, enter the following:

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
- `W` - Save changes
- `Q` - Quit (Will be prompted to save if there are changes)

#### Print line(s)

```bash
cmd:  P1     # Prints line 1
cmd:  P1,10  # Prints lines 1 through 10
cmd:  P1,$   # Prints lines from 1 to end of file
cmd:  P^,10  # Prints lines from beginning to line 10
cmd:  P.,10  # Prints from active line to line 10
cmd:  P.+1   # Print line after active line
```

#### Goto line

```bash
cmd:  G1     # Go to line 1
cmd:  G$     # Go to last line
cmd:  G^     # Go to first line
cmd:  G.+1   # Go to line after active line
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


## Known Issues

List of known issues to be addressed

### Printing lines

- Printing backwards shows same data for all lines printed
    ```bash
    5, cmd: p$,^
    005:  line 5
    004:  line 5
    003:  line 5
    002:  line 5
    001:  line 5
    1, cmd: p^,$
    001:  line 1
    002:  line 2
    003:  line 3
    004:  line 4
    005:  line 5
    5, cmd:
    ```

### Moving lines

- Moving lines when first line is active deletes the moved line(s) and does not reset line numbers

### Deleting lines

- Deleting all lines and then printing results in segmentation fault
- Deleting last line (which loops active line to line 1) and then printing from current line results in segmentation fault
    ```bash
    16, cmd: d16
    1, cmd: p.,$
    Segmentation fault (core dumped)
    ```

### Inserting lines

- Cannot insert lines after the last line (I$+1 maybe?)