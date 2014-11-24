<?
	/*
 	* @class contentManager
 	* @brief Manager de los contenidos
	* @autor David Martos Expósito 
	*
	*/
	
	class userManager
	{
		var $linkdb;
		var $db;
		var $userdb;
		var $passworddb;
 	
		function userManager()
		{
			$this->userdb = USERDB;
		$this->passworddb = PASSWORDDB;
		$this->db = DB;
		$this->linkdb = mysql_connect (MYSQLMACHINE, $this->userdb, $this->passworddb) 
		 	or die ('I cannot connect to the database because: ' . mysql_error());
		
		mysql_select_db ($this->db,$this->linkdb) or die ('No puedo seleccionar la bd'.mysql_error());
		
		
		}
		
		function addUser($info)
		{
			$username = mysql_real_escape_string($info["username"]); 
			$password = $info["password"];
			$mail = $info["mail"];
			$institucion = $info["institucion"];
			$pais = $info["pais"];
			$provincia = $info["provincia"];
			$ciudad = $info["ciudad"];
			$direccion = $info["direccion"];
			$telefono = $info["telefono"];
			$web = $info["web"];
			$ocupacion = $info["ocupacion"];
			$interes = $info["interes"];
			$pais_ins = $info["pais_ins"];
			$provincia_ins = $info["provincia_ins"];
			$ciudad_ins = $info["ciudad_ins"];
			$direccion_ins = $info["direccion_ins"];
			$telefono_ins = $info["telefono_ins"];
			$web_ins = $info["web_ins"];
			$fecha_alta = time();
			
			
			$sql = "INSERT INTO usuarios (username,password,mail,institucion,tipo,ultimo_login,pais,provincia,ciudad,direccion,telefono,web,ocupacion,interes,pais_ins,provincia_ins,ciudad_ins,direccion_ins,telefono_ins,web_ins,fecha_alta)VALUES('$username','$password','$mail','$institucion','user',0,'$pais','$provincia','$ciudad','$direccion','$telefono','$web','$ocupacion','$interes','$pais_ins','$provincia_ins','$ciudad_ins','$direccion_ins','$telefono_ins','$web_ins','$fecha_alta')";
			
			if($result = mysql_query($sql, $this->linkdb))
				return true;
				
			else
				return false;	
		}
		
		function editUser($info,$user)
		{
			
			$username = mysql_real_escape_string($info["username"]); 
			$password = $info["password"];
			$mail = $info["mail"];
			$institucion = $info["institucion"];
			$pais = $info["pais"];
			$provincia = $info["provincia"];
			$ciudad = $info["ciudad"];
			$direccion = $info["direccion"];
			$telefono = $info["telefono"];
			$web = $info["web"];
			$ocupacion = $info["ocupacion"];
			$interes = $info["interes"];
			$pais_ins = $info["pais_ins"];
			$provincia_ins = $info["provincia_ins"];
			$ciudad_ins = $info["ciudad_ins"];
			$direccion_ins = $info["direccion_ins"];
			$telefono_ins = $info["telefono_ins"];
			$web_ins = $info["web_ins"];
			//$user = $info["user"];
			
			$sql = "UPDATE usuarios SET username='$username',password='$password',mail='$mail',institucion='$institucion',pais='$pais',provincia='$provincia',ciudad='$ciudad',direccion='$direccion',telefono='$telefono',web='$web',ocupacion='$ocupacion',interes='$interes',pais_ins='$pais_ins',provincia_ins='$provincia_ins',ciudad_ins='$ciudad_ins',direccion_ins='$direccion_ins',telefono_ins='$telefono_ins',web_ins='$web_ins' WHERE username='$user'";

			//echo "<br>$sql<br>";
			if($result = mysql_query($sql, $this->linkdb))
				return true;
			
			else
				return false;	
		}
		
		function removeUser($username)
		{
			$sql = "DELETE FROM usuarios WHERE username='$username'";
			
			if($result = mysql_query($sql, $this->linkdb))
			{	
				return true;
				mysql_free_result($result);
			}
			else
				return false;	
		}
		
		function login($username, $password)
		{
			$sql = "SELECT COUNT(*) FROM usuarios WHERE username='$username' AND password='$password' ";
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$row = mysql_fetch_array($result);
				mysql_free_result($result);	
			}
			
			if($row[0] == 1)
			{	
				$time = time();
				$sql = "UPDATE usuarios SET ultimo_login='$time' WHERE username='$username'";
				$result = mysql_query($sql, $this->linkdb);
				
				if($_SERVER['HTTP_X_FORWARDED_FOR']!="")
					$ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
				else
					$ip = $_SERVER['REMOTE_ADDR'];	
				
				$sql = "INSERT INTO sesiones (fecha,username,ip) VALUES($time,'$username','$ip')";
				$result = mysql_query($sql, $this->linkdb);
			
				return true;
			}	
				
			else
				return false;
					
		}
		
		function loginAdmin($username, $password)
		{
			$sql = "SELECT COUNT(*) FROM usuarios WHERE username='$username' AND password='$password' AND tipo='admin' ";
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$row = mysql_fetch_array($result);
				mysql_free_result($result);	
			}
			
			if($row[0] == 1)
			{	
				$time = time();
				$sql = "UPDATE usuarios SET ultimo_login='$time' WHERE username='$username'";
				$result = mysql_query($sql, $this->linkdb);
			
				return true;
			}	
				
			else
				return false;
					
		}
		
		function getUser($username)
		{
			$sql = "SELECT * FROM usuarios WHERE username='$username' AND tipo='user'";
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$row = mysql_fetch_array($result);
				$row["num_ejecuciones"] = $this->getNumberExecutions("","",$row["username"]);
				$row["num_sesiones"] = $this->getNumberSessions("","",$row["username"]);
				mysql_free_result($result);	
			}
			
			return $row;
		}
		
		function getUsers($filter,$order)
		{
			$sqlFilter = $this->getSQLFilter($filter);
			$sort = false;
			
			if($order != "")
			{
				if($order["order"]!="num_ejecuciones" && $order["order"]!="num_sesiones"  )
					$sqlOrder = "ORDER BY ".$order["order"]." ". $order["way"];					
				else
					$sort = true;	
			}				
			
			$sql = "SELECT * FROM usuarios WHERE tipo='user'$sqlFilter $sqlOrder";
			//echo $sql;
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$i = 0;
				while($row = mysql_fetch_array($result))
				{
					$users[$i] = $row;
					$users[$i]["num_ejecuciones"] = $this->getNumberExecutions("","",$row["username"]);
					$users[$i]["num_sesiones"] = $this->getNumberSessions("","",$row["username"]);
					if($order["order"] == "num_ejecuciones")
						$vector[$i] = $users[$i]["num_ejecuciones"];
					else if($order["order"] == "num_sesiones")
						$vector[$i] = $users[$i]["num_sesiones"];	
					
					$i++;
				}
				
				if($sort == true)
				{
					if($order["way"] == "ASC")
						asort($vector);
					else
						arsort($vector);	
					
					$j = 0;
					foreach ($vector as $key => $val) 
					{
    					$ordered[$j] = $users[$key];
						$j++; 	
					}
					$users = $ordered;
				}	
				
				mysql_free_result($result);	
			}
			
			return $users;
		}
		
		
	function getNumberExecutions($ini,$fin,$user)
	{
		if($ini!="" && $fin!="")
			$sqlInterval = "AND fecha>=$ini AND fecha<=$fin";
		
		$sql = "SELECT COUNT(*) as num_ejecuciones,u.username FROM usuarios u,ejecuciones e WHERE u.username = e.username AND u.username='$user' AND u.tipo='user' $sqlInterval GROUP BY u.username ORDER BY u.username ";
			
			$num_ejecuciones = 0;
			if($result = mysql_query($sql, $this->linkdb))
			{
			
				if($row = mysql_fetch_array($result))
				{	
					$num_ejecuciones = $row["num_ejecuciones"];
				}
				
				mysql_free_result($result);	
			}
			
			return $num_ejecuciones;
	}
	
	function getNumberSessions($ini,$fin,$user)
	{
		
		if($ini!="" && $fin!="")
			$sqlInterval = "AND fecha>=$ini AND fecha<=$fin";
			
		$sql = "SELECT COUNT(*) as num_sesiones,u.username FROM usuarios u,sesiones s WHERE u.username = s.username AND u.username='$user' AND tipo='user' $sqlInterval GROUP BY u.username ORDER BY u.username ";
			
			$num_sesiones = 0;
			if($result = mysql_query($sql, $this->linkdb))
			{
			
				if($row = mysql_fetch_array($result))
				{	
					$num_sesiones = $row["num_sesiones"];
				}
				
				mysql_free_result($result);	
			}
			
			return $num_sesiones;
	}
	
	function getSQLFilter($filter)
	{
		
		if($filter!="")
		{
			
			$n = false;
			if($filter["pais"] != "0" && $filter["pais"] != "")
			{	
				
				 $sql.=" AND pais='".$filter["pais"]."'";
			
			}				 
		
			if($filter["ocupacion"]!= "0" && $filter["ocupacion"]!= "")
			{	
				
				$sql .=" AND ocupacion='".$filter["ocupacion"]."'";
			}
			
			if($filter["fecha_alta"]!="")
			{
				
				$sql .= " AND fecha_alta>=".$filter["fecha_alta"]["ini"]." AND "."fecha_alta<=".$filter["fecha_alta"]["fin"];
			}
			
			if($filter["ultimo_login"]!="")
			{
					
				$sql .= " AND ultimo_login>=".$filter["ultimo_login"]["ini"]." AND "."ultimo_login<=".$filter["ultimo_login"]["fin"];
			}			 			 
		}
		return $sql;
	}
	
	function getCountries()
	{
			$sql = "SELECT * FROM country";
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$i = 0;
				while($row = mysql_fetch_array($result))
				{
					$countries[$i] = $row;
					$i++;
				}
				
				mysql_free_result($result);	
			}
			
			return $countries;
	}
	
	function getUsersByGroup($group)
	{
		$sql = "SELECT COUNT(*) as num_users,$group FROM usuarios WHERE tipo='user' GROUP BY $group ORDER BY num_users";
			
			if($result = mysql_query($sql, $this->linkdb))
			{
			
				$groupped = array();
				while($row = mysql_fetch_array($result))
				{	
					$groups[0] = $row[$group];
					$groups[1] = $row["num_users"];
					array_push($groupped,$groups);
					
				}
				
				mysql_free_result($result);	
			}
			
			return $groupped;
	}
	
}
	

?>
