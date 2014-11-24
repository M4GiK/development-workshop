<?
	
	
	function listJobs()
	{
		exec("crontab -l",$output,$return);
		print_r($output);
	}	
	
	function installJobs($inform, $file)
	{
		$cron = "* * * * * lynx -dump http://weboptserv.lsi.upc.edu/WEBGRID/admin/notification.php\n";
		if($inform == "S")
				$cron.="59 23 * * * lynx -dump http://weboptserv.lsi.upc.edu/WEBGRID/admin/inform.php\n";
		
		$fd = fopen($file,"w");
		fputs($fd,$cron,4096);
		fclose($fd);
		
		exec("crontab $file",$output,$return);
		print_r($output);
		echo "return: $return";
		 		
	}
	
	function removeJobs()
	{
		exec("crontab -r",$output,$return);
	}

?>