<?
	include "../../frontend/userAccount/parseResult.php";
	
	
	//$exe = generateExecution($_POST);
	if(isset($_GET["file"]))
	{
		
		$output = "../../sim/output/".$_GET["file"].".txt";
		$file = read_file($output);
		$result = parse($file);
		showResult($result);
	}	
	
	echo "<br/><br/><a href=\"index.php\">volver</a>";
	
	
?>
