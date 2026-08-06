# CPU

## MIPSアーキテクチャを用いたCPUの作成

## 実行方法
Asm.ipynbのASMFILEを各テキストファイルに書き換え、実行。sample.cのprogram_pathを作成した.binファイルに書き換え、ターミナル上でmake実行。
``` python
ASMFILE = '(実行したい命令ファイル).txt'

with open('(実行したい命令ファイル).bin', 'wb') as f:
  for c in Codes:
    print(format(c,'#034b'))
    f.write(c.to_bytes(4, byteorder='big'))
```

``` c
int main(int argc, char *argv[]) {
    const char *program_path = (argc > 1) ? argv[1] : "../Assembler/(実行したい命令ファイル).bin";
```
