<?
	/*
 	* @class contentManager
 	* @brief Manager de los contenidos
	* @autor David Martos Expósito 
	*
	*/
	
	class incidenceManager
	{
		var $linkdb;
		var $db;
		var $userdb;
		var $passworddb;
 	
		function incidenceManager()
		{
			$this->userdb = USERDB;
		$this->passworddb = PASSWORDDB;
		$this->db = DB;
		$this->linkdb = mysql_connect (MYSQLMACHINE, $this->userdb, $this->passworddb) 
		 	or die ('I cannot connect to the database because: ' . mysql_error());
		
		mysql_select_db ($this->db,$this->linkdb) or die ('No puedo seleccionar la bd'.mysql_error());
		
		
		}
		
		function addIncidence($incidencia,$user)
		{
			$incidencia = mysql_real_escape_string($incidencia);
			$fecha  = time(); 
			
			$sql = "INSERT INTO incidencias (incidencia,usuario,status,respuesta,fecha)VALUES('$incidencia','$user','espera','espera..',$fecha)";
			
			if($result = mysql_query($sql, $this->linkdb))
				return true;
				
			else
				return false;	
		}
		
		function answerIncidence($id,$info)
		{	
			$status = $info["status"];
			$respuesta = $info["respuesta"];
			$sql = "UPDATE incidencias SET status='$status',respuesta='$respuesta' WHERE id_incidencia='$id'";
			
			if($result = mysql_query($sql, $this->linkdb))
				return true;
			
			else
				return false;	
		}
		
		function removeIncidence($id)
		{
			$sql = "DELETE FROM incidencias WHERE id_incidencia='$id'";
			
			if($result = mysql_query($sql, $this->linkdb))
			{	
				return true;
				mysql_free_result($result);
			}
			else
				return false;	
		}
		
		
		function getIncidence($id)
		{
			$sql = "SELECT * FROM incidencias WHERE id_incidencia='$id'";
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$row = mysql_fetch_array($result);
				mysql_free_result($result);	
			}
			
			return $row;
		}
		
		function getUserIncidences($user)
		{
			$sql = "SELECT * FROM incidencias WHERE usuario='$user' ORDER BY id_incidencia DESC";
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$i = 0;
				while($row = mysql_fetch_array($result))
				{
					$incidences[$i] = $row;
					$i++;
				}
				
				mysql_free_result($result);	
			}
			
			return $incidences;
		}
		
		
		function getIncidences()
		{
			$sql = "SELECT * FROM incidencias ORDER BY id_incidencia DESC";
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$i = 0;
				while($row = mysql_fetch_array($result))
				{
					$incidences[$i] = $row;
					$i++;
				}
				
				mysql_free_result($result);	
			}
			
			return $incidences;
		}
		
		function getNumberWaitingIncidences()
		{
			$sql = "SELECT COUNT(*) AS num FROM incidencias WHERE status='espera'";
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				if($row = mysql_fetch_array($result))
				{
					$num = $row["num"];
				}
				
				mysql_free_result($result);	
			}
			
			return $num;
		}
		
		
	}

?>

