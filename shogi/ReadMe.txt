

将棋連続対局ソフト

コマンドラインでUSIプロトコル対応の将棋エンジンの連続対局ができます。

■使い方

shogi <先手エンジン>  <後手エンジン> [オプション]
オプション:
  -B <設定ファイル>   先手設定フィアル
  -W <設定ファイル>   後手設定フィアル
  -T <持ち時間(秒)> [秒読み(秒)]  持ち時間設定
  -G <num>            対局数
  -M <num>            最大手数
  -P                  PonderをONにする
  -P-                 PonderをOFFにする
  -S                  先後入れ替えをONにする
  -S-                 先後入れ替えをOFFにする
  -L [ログ保存パス]   ログ出力をON、ログ保存パスを設定する
  -L-                 ログ出力をOFFにする
  -N [棋譜保存パス]   棋譜出力ON、棋譜を保存するパスを設定する
  -N-                 棋譜出力OFFにする
  -K <棋譜> [手数]    開始局面を指定した棋譜ファイルの手数からにする

・例 ひよこ将棋同士を、時間0.2秒 対局数10回実行する。
shogi.exe hiyoko.exe hiyoko.exe -T 0 0.2 -G 10



■メモ

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


