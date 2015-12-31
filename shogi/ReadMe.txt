

将棋のテストコード

ソースコードの文字コードはUTF-8 BOM付き
実行時の文字コードはSJIS

vcはマルチバイト文字に設定
gccは--exec-charset=cp932 をつかう

内部で保存する文字列はwstringのUnicode16にする？(USC16?)

テストにはgtestを使う。
テストするときは構成をTestにしてビルドする。
構成をTestにするとプロジェクトshogi側がライブラリ、shogi-test側がexeになる。（普段は逆）


子プロセステスト用のtchidはDebugビルドするとTestにコピーされる。
Testで普通にビルドしてTestにexeを配置するようにすると、
gtestのアドインが正常に動作しないのか、
テストエクスプローラーにテストが出て行こない。
＆なぜか闇にtchildが起動されていて動作がへん？





