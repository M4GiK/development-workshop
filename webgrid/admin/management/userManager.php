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
			$this->linkdb = mysql_connect ("localhost", $this->userdb, $this->passworddb) 
			 	or die ('I cannot connect to the database because: ' . mysql_error());
			
			mysql_select_db ($this->db,$this->linkdb) or die ('No puedo seleccionar la bd'.mysql_error());
		
		}
		
		function addUser($info)
		{
			$username = $info["username"]; 
			$password = $info["password"];
			$mail = $info["mail"];
			$institucion = $info["institucion"];
			
			
			$sql = "INSERT INTO usuarios (username,password,mail,institucion,tipo,ultimo_login)VALUES('$username','$password','$mail','$institucion','user',0)";
			
			if($result = mysql_query($sql, $this->linkdb))
				return true;
				
			else
				return false;	
		}
		
		function editUser($info,$user)
		{
			
			$username = $info["username"]; 
			$password = $info["password"];
			$mail = $info["mail"];
			$institucion = $info["institucion"];
			$sql = "UPDATE usuarios SET username='$username',password='$password',mail='$mail',institucion='$institucion' WHERE username='$user'";

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
		
		function getUsers()
		{
			$sql = "SELECT * FROM usuarios";
			
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
