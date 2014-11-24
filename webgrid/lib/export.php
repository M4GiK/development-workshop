<?
	require("File.php");
	include("../admin/config.php");
	include("../management/configManager.php");
	
	if(isset($_GET["file"]) && isset($_GET["type"]) && isset($_GET["export"]))
	{
		$cm = new configManager();
		$message = $cm->getMessageAdmin();
		
		$file = new File("../sim/output/".$_GET["file"],$_GET["type"]);
		switch($_GET["export"])
		{
			case "pdf":
						$file->exportAsPDF($message);
						break;
			case "xls":
						if($_GET["trace"] == "")
							$trace = 0;
						else
							$trace = $_GET["trace"];
															
						$file->exportAsXLS($trace,$message);
						break;
		}
	}

?>