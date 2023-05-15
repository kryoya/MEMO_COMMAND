# MEMO_COMMAND
 
UNIX上で簡単に手早くメモを取ることができるコマンドを作成しました.
 
 
# Requirement
 
C言語を動かせる環境があれば大丈夫です.
 
 
# Installation
 
1.ホームディレクトリ直下に.memo_projectをダウンロードしてください.
2.カレントディレクトリ(.memo_project)にて下のコマンドを実行してください.
```bash
export PATH=$PATH:$(pwd)
```


# Usage
 
memoコマンドは-make(-m), -search(-s), -add(-a)のオプションがあります.
辞書形式のファイルとなっているので, keyを指定して検索をかけることができます.


##-make

```bash
memo -make 新規ファイル名
```
新規ファイルは.memo_project上に作成され, setting_fileにファイル名が記録されます.


##-search

```bash
memo 指定ファイル名 -search 検索key名
```

ファイル名を指定しない場合, generalが選択されます.
また, -makeオプションを通さず作成されたファイルは指定することができません.


##-add

```bash
memo 指定ファイル名 -add 登録key名 登録word名
```

先ほどと同様にファイル名を指定しない場合, generalが選択されます.
登録key名を省略することができ, その場合はそのファイルのインデックスがkey名となります.


##オプションなし

```bash
memo 指定ファイル名
```

オプションを選択せず, ファイル名だけを指定した場合はそのファイルの中身が表示されます.


# Note
 
まだバグ, 改善点が多いと思いますので何かありましたら連絡をください.
 
 
# Author
 
* 作成者 KUBO Ryoya
* 所属 National Institute of Technology, Kurume College of Department of Electrical and Electronic Engineering
* E-mail ryo.32011023@gmail.com
 
 
# License
当ライセンスはMITライセンスの規約に基づいて付与されています.
 
