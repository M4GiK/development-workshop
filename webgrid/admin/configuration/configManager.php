<?
	/*
 	* @class contentManager
 	* @brief Manager de los contenidos
	* @autor David Martos Expósito 
	*
	*/
	
	class configManager
	{
		var $linkdb;
		var $db;
		var $userdb;
		var $passworddb;
 	
		function configManager()
		{
			$this->userdb = USERDB;
		$this->passworddb = PASSWORDDB;
		$this->db = DB;
		$this->linkdb = mysql_connect (MYSQLMACHINE, $this->userdb, $this->passworddb) 
		 	or die ('I cannot connect to the database because: ' . mysql_error());
		
		mysql_select_db ($this->db,$this->linkdb) or die ('No puedo seleccionar la bd'.mysql_error());
		
		
		}
		
		function editDefaultParams($params)
		{
			$str_params = $this->ArrayParamsToString($params);
			$sql = "UPDATE config SET valor='$str_params' WHERE parametro='default_params'";
			
			if($result = mysql_query($sql, $this->linkdb))
				return true;
			
			else
				return false;	
		}
		
		function getDefaultParams()
		{
			$sql = "SELECT valor FROM config WHERE parametro='default_params'"; 
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$row = mysql_fetch_array($result);
				$str_default = $row["valor"];
				mysql_free_result($result);	
			}
			
			return $this->buildArrayParams($str_default);

		}
		
		function ArrayParamsToString($array)
		{
			$n = false;
			foreach($array as $key=>$value)
			{
				if($n == true)
					$str .= " $key $value";
				else
				{
					$str .= "$key $value";
					$n = true;
				}	
			}
			return $str;
		}
		
		function buildArrayParams($string)
		{
			$array = explode(" ",$string);
			$result = array();
			$i = 0;
			while($i<count($array))
			{
				$result[$array[$i]] = $array[$i+1];
				$i+=2;
			}
			return $result;
		}
		
	
		function getMail()
		{
		
		}
		
		function editMail()
		{
			
		}
		
		function editPassword()
		{
		
		}
		
		
		
		
	}

?>

