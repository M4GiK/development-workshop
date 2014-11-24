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
			
			
			$sql = "INSERT INTO usuarios (username,password,mail,institucion,tipo,ultimo_login,pais,provincia,ciudad,direccion,telefono,web,ocupacion,interes,pais_ins,provincia_ins,ciudad_ins,direccion_ins,telefono_ins,web_ins)VALUES('$username','$password','$mail','$institucion','user',0,'$pais','$provincia','$ciudad','$direccion','$telefono','$web','$ocupacion','$interes','$pais_ins','$provincia_ins','$ciudad_ins','$direccion_ins','$telefono_ins','$web_ins')";
			
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
			$user = $info["user"];
			
			$sql = "UPDATE usuarios SET username='$username',password='$password',mail='$mail',institucion='$institucion',pais='$pais',provincia='$provincia',ciudad='$ciudad',direccion='$direccion',telefono='$telefono',web='$web',ocupacion='$ocupacion',interes='$interes',pais_ins='$pais_ins',provincia_ins='$provincia_ins',ciudad_ins='$ciudad_ins',direccion_ins='$direccion_ins',telefono_ins='$telefono_ins',web_ins='$web_ins' WHERE username='$user'";

			echo "<br>$sql<br>";
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
			$sql = "SELECT * FROM usuarios WHERE username='$username'";
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$row = mysql_fetch_array($result);
				mysql_free_result($result);	
			}
			
			return $row;
		}
		
		function getUsers($filter,$info)
		{
			$sql = "SELECT COUNT(*) as num_ejecuciones,u.username,u.fecha_alta,u.ultimo_login,u.pais,u.ocupacion FROM usuarios u,ejecuciones e,sesiones s WHERE u.username = e.username GROUP BY username ORDER BY username ";
			
			echo "<br>$sql<br>";
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$i = 0;
				while($row = mysql_fetch_array($result))
				{
					$users[$i] = $row;
					$i++;
				}
				
				mysql_free_result($result);	
			}
			
			return $users;
		}
		
		function logout($username)
		{
			
		}
	}

?>
