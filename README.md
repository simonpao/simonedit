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
text/text.simon:1$: P
001:  line 1
002:  line 2
003:  line 3
004:  line 4
005:  line 5
text/text.simon:5$: 
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
text/text.simon:1$: P      # Prints all lines of the file (same as P^,$)
text/text.simon:1$: P1     # Prints line 1
text/text.simon:1$: P1,10  # Prints lines 1 through 10
text/text.simon:1$: P1,$   # Prints lines from 1 to end of file
text/text.simon:1$: P^,10  # Prints lines from beginning to line 10
text/text.simon:1$: P.,10  # Prints from active line to line 10
text/text.simon:1$: P.+1   # Print line after active line
text/text.simon:1$: P.-1   # Print line before active line
```

#### Goto line

```bash
text/text.simon:1$: G1     # Go to line 1
text/text.simon:1$: G$     # Go to last line
text/text.simon:1$: G^     # Go to first line
text/text.simon:1$: G.+1   # Go to line after active line
text/text.simon:1$: G.-1   # Go to line before active line
```

#### Move line

```bash
text/text.simon:1$: M1     # Move line 1
text/text.simon:1$: M1,5   # Move lines 1 through 5
text/text.simon:1$: M^,5   # Move first through fifth lines
text/text.simon:1$: M5,$   # Move fifth through last lines
```

#### Delete line

```bash
text/text.simon:1$: D1     # Delete line 1
text/text.simon:1$: D1,5   # Delete lines 1 through 5
text/text.simon:1$: D^,5   # Delete first through fifth lines
text/text.simon:1$: D5,$   # Delete fifth through last lines
text/text.simon:1$: D.     # Delete active line
text/text.simon:1$: D.+1   # Delete line after active line
text/text.simon:1$: D^,$   # Delete all lines
```

#### Insert line

```bash
text/text.simon:1$: I1     # Insert before line 1
text/text.simon:1$: I      # Insert before active line
```

#### Append to line

```bash
text/text.simon:1$: A1     # Append text to the end of line 1
text/text.simon:1$: A      # Append text to the end of active line
```

#### Replace line

```bash
text/text.simon:1$: R1     # Replace the text of line 1
text/text.simon:1$: R      # Replace the text of active line
```

#### Count lines

```bash
text/text.simon:1$: C      # Count the number of lines in the file (same as C^,$)
text/text.simon:1$: C.,$   # Count the number of lines from the current line to the last line (inclusive)
text/text.simon:1$: C1     # Count the number of characters on line 1
```

#### Save changes

```bash
text/text.simon:1$: W      # Save changes to current file
text/text.simon:1$: Wtext2 # Save changes to a file named 'text2.simon'
```

#### Quit

```bash
text/text.simon:1$: Q      # Close file without saving
File modified. Enter W to save, Q to discard.
text/text.simon:1$: Q
```
