extern exceptionHandle

%macro isrErrStub 1
isrStub%+%1:
    mov rdi, %1
    mov rsi, [rsp]
    mov rdx, [rsp+8]
    mov rcx, [rsp+16]
    mov rbx, [rsp+24]
    call exceptionHandle
    add rsp, 8
    iretq
%endmacro

%macro isrNoErrStub 1
isrStub%+%1:
    mov rdi, %1
    mov rsi, 0
    mov rdx, [rsp]
    mov rcx, [rsp+8]
    mov rbx, [rsp+16]
    call exceptionHandle
    iretq
%endmacro

isrNoErrStub 0
isrNoErrStub 1
isrNoErrStub 2
isrNoErrStub 3
isrNoErrStub 4
isrNoErrStub 5
isrNoErrStub 6
isrNoErrStub 7
isrErrStub 8
isrNoErrStub 9
isrErrStub 10
isrErrStub 11
isrErrStub 12
isrErrStub 13
isrErrStub 14
isrNoErrStub 15
isrNoErrStub 16
isrErrStub 17
isrNoErrStub 18
isrNoErrStub 19
isrNoErrStub 20
isrErrStub 21
isrNoErrStub 22
isrNoErrStub 23
isrNoErrStub 24
isrNoErrStub 25
isrNoErrStub 26
isrNoErrStub 27
isrNoErrStub 28
isrNoErrStub 29
isrErrStub 30
isrNoErrStub 31

global isrStubTable
isrStubTable:
%assign i 0
%rep 32
    dq isrStub%+i
%assign i i+1
%endrep