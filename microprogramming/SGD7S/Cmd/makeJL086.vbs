Dim obj
Dim FSO 
Dim CurrentPath

Set obj = WScript.CreateObject("Excel.Application")
Set FSO = CreateObject("Scripting.FileSystemObject") 

CurrentPath = FSO.GetParentFolderName(Wscript.ScriptFullName)

obj.Visible = False

obj.Workbooks.Open (CurrentPath  & "\�p�����[�^�\�[�X�쐬�}�N��.xls")

obj.Application.Run "CreateSource"

obj.Application.DisplayAlerts = False
obj.Workbooks.Close()
obj.Application.DisplayAlerts = True

