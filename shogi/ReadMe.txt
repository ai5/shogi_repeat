

連続対局ソフト

・ソースコードの文字コードはUTF-8 BOM付き

・実行時の文字コードはSJIS
 => VC(mingwも？）はSJIS LinuxはUTF-8

・vcはマルチバイト文字に設定
 gccは--exec-charset=cp932 をつかう
 => gccでのcp932指定はやめた

・内部で保存する文字列はwstringのUnicode16にする？(USC2?)
 => VCは UCS-2, LinuxはUCS-4

・テストにはgtestを使う。
テストするときは構成をTestにしてビルドする。
構成をTestにするとプロジェクトshogi側がライブラリ、shogi-test側がexeになる。（普段は逆）


・子プロセステスト用のtchidはDebugビルドするとTestにコピーされる。
Testで普通にビルドしてTestにexeを配置するようにすると、
gtestのアドインが正常に動作しないのか、
テストエクスプローラーにテストが出て行こない。
＆なぜか闇にtchildが起動されていて動作がへん？


・デバッグ時のオプション
持ち時間なし、秒読0.2秒
対局数2
shogi.exe hiyoko.exe hiyoko.exe -T 0 0.2 -G 2

shogi.exe gpsfish/gpsfish.exe hiyoko.exe -T 0 0.2 -G 1 -B black.ini


