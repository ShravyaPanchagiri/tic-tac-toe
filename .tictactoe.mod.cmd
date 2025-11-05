savedcmd_tictactoe.mod := printf '%s\n'   tictactoe.o | awk '!x[$$0]++ { print("./"$$0) }' > tictactoe.mod
