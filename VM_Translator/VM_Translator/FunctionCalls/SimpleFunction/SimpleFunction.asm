// Initializing boot strapper
@256
D=A
@SP
M=D
// Calling Sys.init
@returnAddress_0
D=A
@SP
A=M
M=D
A=A+1
D=A
@SP
M=D
@LCL
D=M
@SP
A=M
M=D
A=A+1
D=A
@SP
M=D
@ARG
D=M
@SP
A=M
M=D
A=A+1
D=A
@SP
M=D
@THIS
D=M
@SP
A=M
M=D
A=A+1
D=A
@SP
M=D
@THAT
D=M
@SP
A=M
M=D
A=A+1
D=A
@SP
M=D
@0
D=A
@5
D=D+A
@SP
D=M-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.init
0;JMP
(returnAddress_0)
(SimpleFunction.test)
@0
D=A
@SP
A=M
M=D
A=A+1
D=A
@SP
M=D
@0
D=A
@SP
A=M
M=D
A=A+1
D=A
@SP
M=D
@LCL
D=M
@0
D=A+D
@R13
M=D
A=M
D=M
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@1
D=A+D
@R13
M=D
A=M
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
A=A-1
M=M+D

@SP
A=M-1
M=!M
@ARG
D=M
@0
D=A+D
@R13
M=D
A=M
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
A=A-1
M=M+D

@ARG
D=M
@1
D=A+D
@R13
M=D
A=M
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
A=A-1
M=M-D

@LCL
D=M
@R14
M=D
@5
AD=D-A
D=M
@R15
M=D
@SP
M=M-1
A=M
D=M
@ARG
A=M
M=D
@ARG
D=M
@SP
M=D+1
@R14
M=M-1
A=M
D=M
@THAT
M=D
@R14
M=M-1
A=M
D=M
@THIS
M=D
@R14
M=M-1
A=M
D=M
@ARG
M=D
@R14
M=M-1
A=M
D=M
@LCL
M=D
@R15
A=M
0;JMP
