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
		
		
		function getIncidences($filter, $order)
		{
			$sqlFilter = $this->getSQLFilter($filter);
			$sort = false;
			
			if($order != "")
				$sqlOrder = "ORDER BY ".$order["order"]." ". $order["way"];	
				
			$sql = "SELECT * FROM incidencias $sqlFilter $sqlOrder";	
			//$sql = "SELECT * FROM incidencias ORDER BY id_incidencia DESC";
			
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
			
				if($filter["usuario"]!= "0" && $filter["usuario"]!= "")
				{	
					if($n == false)
					{	
						$sql.="WHERE ";
						$n = true; 
					}
					else
						$sql.=" AND ";
					
					$sql .="usuario='".$filter["usuario"]."'";
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