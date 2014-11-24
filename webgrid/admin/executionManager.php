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
		
		function newExecution($username,$params)
		{
			
			$fecha = time();
			$output = "$username-$fecha";
			$params .= " --trace $username-trace-$fecha.txt";
			$condor_id = condor_submit($params, $output);
			$sql = "INSERT INTO ejecuciones (username,fecha,parametros,condor_id,status)VALUES('$username',$fecha,'$params','$condor_id','running')";
			
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
			
			
			for($i = 0; $i < count($executions);$i++)
			{
				
				if(condor_q($executions[$i]["condor_id"]))
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
			
			
			for($i = 0; $i < count($executions);$i++)
			{
				
				if(condor_q($executions[$i]["condor_id"]))
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
			$sql = "SELECT * FROM ejecuciones WHERE username='$username'";
			
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
		
		function getExecutions()
		{
			$sql = "SELECT * FROM ejecuciones";
			
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
		
		
	}

?>
