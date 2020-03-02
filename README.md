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
cmd:  p1,5
1  line 1
2  line 2
3  line 3
4  line 4
5  line 5
```

Available commands are:

- `P#[,#]` - Print line # [through #]
- `D#[,#]` - Delete line # [through #]
- `I#` - Insert line #???, `.` to end
- `M` - Move (Not sure about this one yet)
- `W` - Save Changes
- `Q` - Quit (Will be prompted to save if there are changes)