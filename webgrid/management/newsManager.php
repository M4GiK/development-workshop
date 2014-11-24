<?
	/*
 	* @class newsManager
 	* @brief Manager de los contenidos
	* @autor David Martos Expósito 
	*
	*/
	
	class newsManager
	{
		var $linkdb;
		var $db;
		var $userdb;
		var $passworddb;
 	
		function newsManager()
		{
			$this->userdb = USERDB;
		$this->passworddb = PASSWORDDB;
		$this->db = DB;
		$this->linkdb = mysql_connect (MYSQLMACHINE, $this->userdb, $this->passworddb) 
		 	or die ('I cannot connect to the database because: ' . mysql_error());
		
		mysql_select_db ($this->db,$this->linkdb) or die ('No puedo seleccionar la bd'.mysql_error());
		
		
		}
		
		function addNew($info)
		{
			$titular_es = $info["titular_es"];			
			$titular_en = $info["titular_en"];
			$titular_ca = $info["titular_ca"];
			$cuerpo_es = $info["cuerpo_es"];
			$cuerpo_en = $info["cuerpo_en"];
			$cuerpo_ca = $info["cuerpo_ca"];
			$fecha_exp = $info["fecha_exp"];
			$fecha = time();
			
			$sql = "INSERT INTO noticias (titular_es,titular_en,titular_ca,cuerpo_es,cuerpo_en,cuerpo_ca,fecha_exp,fecha)VALUES('$titular_es','$titular_en','$titular_ca','$cuerpo_es','$cuerpo_en','$cuerpo_ca',$fecha_exp,$fecha)";
			
			if($result = mysql_query($sql, $this->linkdb))
				return true;
				
			else
				return false;	
		}
		
		function editNew($info,$id)
		{
			
			$titular_es = $info["titular_es"];			
			$titular_en = $info["titular_en"];
			$titular_ca = $info["titular_ca"];
			$cuerpo_es = $info["cuerpo_es"];
			$cuerpo_en = $info["cuerpo_en"];
			$cuerpo_ca = $info["cuerpo_ca"];
			$fecha_exp = $info["fecha_exp"];
			
			$sql = "UPDATE noticias SET titular_es='$titular_es',titular_en='$titular_en',titular_ca='$titular_ca',cuerpo_es='$cuerpo_es',cuerpo_en='$cuerpo_en',cuerpo_ca='$cuerpo_ca',fecha_exp=$fecha_exp WHERE id_noticia=$id";
			

			if($result = mysql_query($sql, $this->linkdb))
				return true;
			
			else
				return false;	
		}
		
		function removeNew($id)
		{
			$sql = "DELETE FROM noticias WHERE id_noticia=$id";
			
			if($result = mysql_query($sql, $this->linkdb))
			{	
				return true;
				mysql_free_result($result);
			}
			else
				return false;	
		}
		
		
		
		function getNew($id)
		{
			$sql = "SELECT * FROM noticias WHERE id_noticia=$id";
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$row = mysql_fetch_array($result);
				mysql_free_result($result);	
			}
			
			return $row;
		}
		
		function getNews($time)
		{
			if($time == "")
				$sql = "SELECT * FROM noticias";
			else
				$sql = "SELECT * FROM noticias WHERE fecha_exp>=$time ORDER BY fecha_exp DESC";
			
		
			if($result = mysql_query($sql, $this->linkdb))
			{
				$i = 0;
				while($row = mysql_fetch_array($result))
				{
					$news[$i] = $row;
					$i++;
				}
				
				mysql_free_result($result);	
			}
			
			return $news;
		}
		
		
	}

?>
