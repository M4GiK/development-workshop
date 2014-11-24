<?
	/*
 	* @class contentManager
 	* @brief Manager de los contenidos
	* @autor David Martos Expósito 
	*
	*/
	
	class executionManager
	{
		var $linkdb;
		var $db;
		var $userdb;
		var $passworddb;
 	
		function executionManager()
		{
			$this->userdb = USERDB;
			$this->passworddb = PASSWORDDB;
			$this->db = DB;
			$this->linkdb = mysql_connect (MYSQLMACHINE, $this->userdb, $this->passworddb) 
		 	or die ('I cannot connect to the database because: ' . mysql_error());
		
		mysql_select_db ($this->db,$this->linkdb) or die ('No puedo seleccionar la bd'.mysql_error());
		
		
		}
		
		function newExecution($username,$params, $status_mail)
		{				
			$fecha = time();
			$output = "$username-$fecha";
			//$params .= " --trace $username-trace-$fecha.txt";
			//$condor_id = condor_submit($params, $output);
			$exec =  new Executor($params, $output,"$username-trace-$fecha.txt");
			$result = $exec->execute();
			$tipo = $result["tipo"];
			$condor_id = $result["condor_id"];
			
			if($tipo == "directa")
				$status = "finished";
			else
				$status = "running";	
			
			$sql = "INSERT INTO ejecuciones (username,fecha,parametros,condor_id,status,status_mail,tipo)VALUES('$username',$fecha,'$params','$condor_id','$status','$status_mail','$tipo')";
			
			if($result = mysql_query($sql, $this->linkdb))
				return true;
				
			else
				return false;	
		}
		
		function checkUserExecutionsStatus($username)
		{
			
			$sql = "SELECT * FROM ejecuciones WHERE status='running'";
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$i = 0;
				while($row = mysql_fetch_array($result))
				{
					$executions[$i] = $row;
				}
				
				mysql_free_result($result);	
			}
			
			$exec = new Executor();
			for($i = 0; $i < count($executions);$i++)
			{
				
				if($exec->condor_q($executions[$i]["condor_id"]))
				{
					$user = $executions[$i]["username"];
					$fecha = $executions[$i]["fecha"];
					$update = "UPDATE ejecuciones SET status='finished' WHERE username='$user' AND fecha=$fecha";
					$result = mysql_query($update, $this->linkdb);
				}	
			}
			
		}
		
		function checkExecutionsStatus()
		{
			
			$sql = "SELECT * FROM ejecuciones";
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$i = 0;
				while($row = mysql_fetch_array($result))
				{
					$executions[$i] = $row;
				}
				
				mysql_free_result($result);	
			}
			
			$exec = new Executor();
			for($i = 0; $i < count($executions);$i++)
			{
				
				if($exec->condor_q($executions[$i]["condor_id"]))
				{
					$user = $executions[$i]["username"];
					$fecha = $executions[$i]["fecha"];
					$update = "UPDATE ejecuciones SET status='finished' WHERE username='$user' AND fecha=$fecha";
					$result = mysql_query($update, $this->linkdb);
				}	
			}
		}
		function removeExecution($username, $time)
		{
			$sql = "DELETE FROM ejecuciones WHERE username='$username' and fecha=$time";
			
			if($result = mysql_query($sql, $this->linkdb))
			{	
				// borramos los ficheros asociados
				unlink(OUTPATH."/$username-$time.txt");
				unlink(OUTPATH."/$username-trace-$time.txt");
				return true;
				mysql_free_result($result);
			}
			else
				return false;	
		}
		
		
		
		function getUserExecutions($username)
		{
			
			$sql = "SELECT * FROM ejecuciones WHERE username='$username' ORDER BY fecha DESC";
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$i = 0;
				while($row = mysql_fetch_array($result))
				{
					$executions[$i] = $row;
					$i++;
				}
				mysql_free_result($result);	
			}
			
			return $executions;
		}
		
		function getExecutions($filter, $order)
		{
			$sqlFilter = $this->getSQLFilter($filter);
			$sort = false;
			
			if($order != "")
				$sqlOrder = "ORDER BY ".$order["order"]." ". $order["way"];					
				
			
			$sql = "SELECT * FROM ejecuciones $sqlFilter $sqlOrder";
			//echo $sql;
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$i = 0;
				while($row = mysql_fetch_array($result))
				{
					$executions[$i] = $row;
					$i++;
				}
				mysql_free_result($result);	
			}
			
			return $executions;
		}
		
		function getSQLFilter($filter)
		{
		
			if($filter!="")
			{
				
				$n = false;
				if($filter["status"] != "0" && $filter["status"] != "")
				{	
					if($n == false)
					{	
						$sql.="WHERE ";
						$n = true; 
					}
					else
						$sql.=" AND ";
						
					 $sql.="status='".$filter["status"]."'";
				
				}				 
			
				if($filter["username"]!= "0" && $filter["username"]!= "")
				{	
					if($n == false)
					{	
						$sql.="WHERE ";
						$n = true; 
					}
					else
						$sql.=" AND ";
					
					$sql .="username='".$filter["username"]."'";
				}
				
				if($filter["fecha"]!="")
				{
					if($n == false)
					{	
						$sql.="WHERE ";
						$n = true; 
					}
					else
						$sql.=" AND ";
						
					$sql .= "fecha>=".$filter["fecha"]["ini"]." AND "."fecha<=".$filter["fecha"]["fin"];
				}
				
			}
			return $sql;
		}
		
		function getExecutionsByGroup($group)
		{
			$sql = "SELECT COUNT(*) as num_status,$group FROM ejecuciones GROUP BY $group ORDER BY username";
				
				if($result = mysql_query($sql, $this->linkdb))
				{
				
					$groupped = array();
					while($row = mysql_fetch_array($result))
					{	
						$groups[0] = $row[$group];
						$groups[1] = $row["num_status"];
						array_push($groupped,$groups);
						
					}
					
					mysql_free_result($result);	
				}
				
				return $groupped;
		}
		
		function checkMailPendingExecutions()
		{
			$pending = array();
			$sql = "SELECT * FROM ejecuciones WHERE status_mail='P' AND status='finished'";
			echo $sql;
				
				if($result = mysql_query($sql, $this->linkdb))
				{
				
					
					while($row = mysql_fetch_array($result))
					{	
						$pending[] = $row;
						
					}
					
					mysql_free_result($result);	
				}
				
				return $pending;
		}	
		
		function updateMailPendingExecution($username, $fecha)
		{
			$sql = "UPDATE ejecuciones SET status_mail='S' WHERE username='$username' AND fecha='$fecha'";
			echo "<br>$sql<br>";
				if($result = mysql_query($sql, $this->linkdb))
				{	
					return $sql;
				}
				else
				{	
					return false;
				}	
		}
		
	}

?>
