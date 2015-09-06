7-zip32.dllテストプログラムメモ書き

DLL選択
  出力C：LoadLibraryの戻り値

7-zip32
  入力 ：DLLに送信するコマンド
  出力C：APIの戻り値

GetVersion
  出力C：APIの戻り値

GetSubVersion
  出力C：APIの戻り値

GetRunning
  出力C：APIの戻り値

ConfigDialog
  出力A：_szOptionBuffer の内容
  出力C：APIの戻り値
  
CheckArchive
  入力 ：ファイル名
  出力C：APIの戻り値

FunctionList
  入力 ：APIを示す一様な数値
  出力C：APIの戻り値

GetFileCount
  入力 ：ファイル名
  出力C：APIの戻り値

SetOwnerWindow
  入力 ：ファイル名
  出力C：APIの戻り値

ClearOwnerWnd
  入力 ：ファイル名
  出力C：APIの戻り値

SetOwnerWndEx
  入力 ：ファイル名
  出力C：APIの戻り値

KillOwnerWndEx
  入力 ：ファイル名
  出力C：APIの戻り値

OpenArchive
  入力 ：ファイル名
  出力C：APIの戻り値

FindFirst
  入力 ：_szWildName
  出力B：_lpSubInfo の内容
  出力C：APIの戻り値

FindNext
  出力B：_lpSubInfo の内容
  出力C：APIの戻り値

CloseArchive
  出力C：APIの戻り値

GetArcFileName
  出力B：_lpBuffer の内容
  出力C：APIの戻り値

GetArcFileSize
  出力C：APIの戻り値

GetArcOrgSize
  出力C：APIの戻り値

GetArcCmpSize
  出力C：APIの戻り値

GetArcRatio
  出力C：APIの戻り値

GetArcDate
  出力C：日付に変換されたAPIの戻り値

GetArcTime
  出力C：時間に変換されたAPIの戻り値

GetArcOSType
  出力C：APIの戻り値

IsSFXFile
  出力C：APIの戻り値

GetFileName
  出力A：_lpBuffer の内容
  出力C：APIの戻り値

GetOriginalSize
  出力C：APIの戻り値

GetCmpSize
  出力C：APIの戻り値

GetRatio
  出力C：APIの戻り値

GetDate
  出力C：日付に変換されたAPIの戻り値

GetTime
  出力C：時間に変換されたAPIの戻り値

GetCRC
  出力C：APIの戻り値

GetAttribute
  出力C：APIの戻り値

GetOSType
  出力C：APIの戻り値

GetMethod
  出力A：_lpBuffer の内容
  出力C：APIの戻り値

GetWriteTime
  出力C：日時に変換されたAPIの戻り値

GetWriteTimeEx
  出力C：日時に変換されたAPIの戻り値

ArcWriteTimeEx
  出力C：日時に変換されたAPIの戻り値

ArcCreateTimeEx
  出力C：日時に変換されたAPIの戻り値

ArcAccessTimeEx
  出力C：日時に変換されたAPIの戻り値

GetArcFileSizeEx
  出力A：_llSize の内容
  出力C：APIの戻り値

GetArcOrgSizeEx
  出力A：_llSize の内容
  出力C：APIの戻り値

GetArcCmpSizeEx
  出力A：_llSize の内容
  出力C：APIの戻り値

GetOriginalSizeEx
  出力A：_llSize の内容
  出力C：APIの戻り値

GetCmpSizeEx
  出力A：_llSize の内容
  出力C：APIの戻り値