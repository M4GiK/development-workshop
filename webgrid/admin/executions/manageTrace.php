<?
	include "../../frontend/userAccount/parseResult.php";
	
	
	//$exe = generateExecution($_POST);
	echo "<h1>Trace</h1>";
	if(isset($_GET["file"]))
	{
		
		$output = "../../sim/output/".$_GET["file"].".txt";
		$file = read_file($output);
		$result = parseTrace($file);
		echo showTrace($result);
	}	
	
    
	echo "<a href=\"index.php\">volver</a>";
	
	
?>