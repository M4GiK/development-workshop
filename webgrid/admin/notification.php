<?
	include "config.php";
	include "../management/userManager.php";
	include "../management/executionManager.php";
	include "../management/configManager.php";
	require "../lib/phpmailer/class.phpmailer.php";
	include "../lib/Mail.php";
	include "../lib/File.php";
	
	
	$em = new executionManager();
	$um = new userManager();
	$cm = new configManager();
	$message = "<p><strong>".$cm->getMessageAdmin()."</strong></p>";
	
	$pending = $em->checkMailPendingExecutions();
	
	print_r($pending);
	echo "hola";
	$fd = fopen("cronfile.txt","a");
	
	for($i = 0; $i < count($pending); $i++)
	{
		$user = $um->getUser($pending[$i]["username"]);
		$to = $user["mail"];
		$message .= "<p>The execution of ".date("G:i d/m/Y",$pending[$i]["fecha"])." finished succesfully</p>\n";
		$alt_message = "hola";
		$username = $pending[$i]["username"];
		$date = $pending[$i]["fecha"];
		
		//result
		$path = OUTPATH."/$username-$date.txt";
		$file = new File($path, "result");
		$message.= $file->getHTML()."\n";
		
		//trace
		$path = OUTPATH."/$username-trace-$date.txt";
		$file = new File($path, "trace");
		$traces = $file->getHTML(0,false);
		$message .= $traces["html"];
		
		for($i = 1; $i < $traces["total"]; $i++)
		{
			$traces = $file->getHTML($i,false);
			$message .= $traces["html"];
		}
		
		//$alt_message = $message;
		echo "$to,$message";
		$mail = new Mail("WEB GRID @ UPC",$to,"Execution results",$message,$alt_message, "../lib/phpmailer/");
		$mail->send();
		$sql = $em->updateMailPendingExecution($username, $date);
		fputs($fd,"ejecución:".time().":\n",4096);
		fputs($fd,"$sql\n",4096);
		fclose($fd);
	}
	
	
?>
