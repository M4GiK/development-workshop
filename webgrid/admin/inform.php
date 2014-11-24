<?
		include "../management/userManager.php";
		include "../management/executionManager.php";
		include "../management/incidenceManager.php";
		require "../lib/phpmailer/class.phpmailer.php";
		include "../lib/Mail.php";
		include "config.php";
		include "../management/configManager.php";
		
		function getNewUsers($ini, $fin)
		{
			$filter["fecha_alta"]["ini"] = $ini;
			$filter["fecha_alta"]["fin"] = $fin;
			$userm = new userManager();
			$users = $userm->getUsers($filter,"");
			print_r($users);
			return count($users);
		}
		function getNewExecutions($ini, $fin)
		{
			$filter["fecha"]["ini"] = $ini;
			$filter["fecha"]["fin"] = $fin;
			$em = new executionManager();
			$executions = $em->getExecutions($filter,"");
			print_r($executions);
			
			return count($executions);
			
		}
		function getNewIncidences($ini, $fin)
		{
			$filter["fecha"]["ini"] = $ini;
			$filter["fecha"]["fin"] = $fin;
			$im = new incidenceManager();
			$incidences = $im->getIncidences($filter,"");
			print_r($incidences);
			return count($incidences);
		}
		
		$actual = getdate(time());
		$ini = mktime(0,0,0,$actual["mon"],$actual["mday"],$actual["year"]);
		$fin = mktime(23,59,59,$actual["mon"],$actual["mday"],$actual["year"]);
		
		$new_users = getNewUsers($ini, $fin);
		$new_executions = getNewExecutions($ini, $fin);
		$new_incidences = getNewIncidences($ini, $fin);
		
		$cm = new configManager();
		$to = $cm->getMail();
		$subject ="Resumen del dia:"."  ".$actual["mday"]."/".$actual["mon"]."/".$actual["year"];
		$message .= "<p><strong> Resumen del dia ".$actual["mday"]."/".$actual["mon"]."/".$actual["year"].":</strong></p>\n";
		$message .= "<p><strong> usuarios nuevos:</strong> $new_users</p>\n";
		$message .= "<p><strong> ejecuciones lanzadas:</strong> $new_executions</p>\n";
		$message .= "<p><strong> incidencias nuevas:</strong> $new_incidences</p>\n";
		$alt_message = "hola";
		
		echo "$subject, $to,$message";
		$mail = new Mail("WEB GRID",$to,$subject,$message,$alt_message, "../lib/phpmailer/");
		$mail->send();
		
		
		
		
?>
