<?

/*
 * @class contentManager
 * @brief Manager de los contenidos
 * @autor David Martos Expósito 
 *
 */
 
 class contentManager
 {
 	var $linkdb;
	var $db;
	var $userdb;
	var $passworddb;
 	
	function contentManager()
	{
		$this->userdb = USERDB;
		$this->passworddb = PASSWORDDB;
		$this->db = DB;
		$this->linkdb = mysql_connect ("localhost", $this->userdb, $this->passworddb) 
		 	or die ('I cannot connect to the database because: ' . mysql_error());
		
		mysql_select_db ($this->db,$this->linkdb) or die ('No puedo seleccionar la bd'.mysql_error());
		
	}
	
	// CONSULTORAS
	
	function getContent($section,$subsection,$lang)
	{
		
		$sql = "";
		switch($lang)
		{
			case "ES":	
						if($subsection == "")
							$sql = "SELECT texto_es FROM secciones WHERE id_seccion='$section'";
						else
							$sql = "SELECT texto_es FROM subsecciones WHERE id_subseccion='$subsection' AND id_seccion='$section'";
						
						break;
			case "EN":		
						if($subsection == "")
							$sql = "SELECT texto_en FROM secciones WHERE id_seccion='$section'";
						else
							$sql = "SELECT texto_en FROM subsecciones WHERE id_subseccion='$subsection' AND id_seccion='$section'";
						break;
			case "CA":
						if($subsection == "")
							$sql = "SELECT texto_ca FROM secciones WHERE id_seccion='$section'";
						else
							$sql = "SELECT texto_ca FROM subsecciones WHERE id_subseccion='$subsection' AND id_seccion='$section'";
						
						break;						
		}
		if($result = mysql_query($sql, $this->linkdb))
		{
			
			$row = mysql_fetch_array($result);
			$text = $row[0];
			mysql_free_result($result);
		}
		
		
		return $text;
	}
	
	function getSection($section, $subsection)
	{
		if($subsection == "")
			$sql = "SELECT * FROM secciones WHERE id_seccion='$section'";
		else
			$sql = "SELECT * FROM subsecciones WHERE id_subseccion='$subsection' AND id_seccion='$section'";	
		
		if($result = mysql_query($sql, $this->linkdb))
			$row = mysql_fetch_array($result);
		
		mysql_free_result($result);
		
		return $row;
	}
	
	function getSectionByName($section)
	{
		
		$sql = "SELECT * FROM secciones WHERE titulo_en='$section'";
		
		if($result = mysql_query($sql, $this->linkdb))
			$row = mysql_fetch_array($result);
		
		mysql_free_result($result);
		
		return $row;
	}
	function getNumberSections()
	{
		$sql = "SELECT COUNT(*) FROM secciones";
		$number = 0;
		if($result = mysql_query($sql, $this->linkdb))
		{
			$row = mysql_fetch_array($result);
			print_r($row);
			$number = $row[0];
			mysql_free_result($result);	
		}
		
		return $number;
	}
	function getSections()
	{

		$sql = "SELECT * FROM secciones ORDER BY id_seccion";
		if($result = mysql_query($sql, $this->linkdb))
		{
			$i = 0;
			while ($row = mysql_fetch_array($result))
			{
				$sections[$i] = $row;
				$i++;
				
			}
			mysql_free_result($result);	
		}
		
		
		return $sections;
	}
	
	function getNumberSubsections($section)
	{
		$sql = "SELECT COUNT(*) FROM subsecciones WHERE id_seccion=$section";
		$number = 0;
		if($result = mysql_query($sql, $this->linkdb))
		{
			$row = mysql_fetch_array($result);
			$number = $row[0];
			mysql_free_result($result);	
		}
		
		return $number;
	}
	function getSubsections($section)
	{
		$sql = "SELECT * FROM subsecciones WHERE id_seccion='$section' ORDER BY id_subseccion";
		
		if($result = mysql_query($sql, $this->linkdb))
		{
			$i = 0;
			while ($row = mysql_fetch_array($result))
			{
				$subsections[$i] = $row;
				$i++;
			}
			mysql_free_result($result);	
		}
		
		
		return $subsections;
	}
	
	
	
	// MODIFICADORAS
	
	function updateSection($section, $subsection, $info)
	{
		$titulo_es = $info["titulo_es"]; 
		$titulo_en = $info["titulo_en"];
		$titulo_ca = $info["titulo_ca"];
		$texto_es = $info["texto_es"];
		$texto_en = $info["texto_en"];
		$texto_ca = $info["texto_ca"];
		$link_externo = $info["link_externo"];
		$link = $info["link"];
		
		if($subsection == "")
			$sql = "UPDATE secciones SET titulo_es='$titulo_es',titulo_en='$titulo_en',titulo_ca='$titulo_ca',texto_es='$texto_es',texto_en='$texto_en',texto_ca='$texto_ca',link_externo='$link_externo',link='$link' WHERE id_seccion='$section'";
		else
			$sql = "UPDATE subsecciones SET titulo_es='$titulo_es',titulo_en='$titulo_en',titulo_ca='$titulo_ca',texto_es='$texto_es',texto_en='$texto_en',link_externo='$link_externo',link='$link' WHERE id_subseccion='$subsection' AND id_seccion='$section'";

		if($result = mysql_query($sql, $this->linkdb))
		{	
			return true;
		}
		else
			return false;	
	}
	function addSubsection($section,$info)
	{
		$titulo_es = $info["titulo_es"]; 
		$titulo_en = $info["titulo_en"];
		$titulo_ca = $info["titulo_ca"];
		$texto_es = $info["texto_es"];
		$texto_en = $info["texto_en"];
		$texto_ca = $info["texto_ca"];
		$link_externo = $info["link_externo"];
		$link = $info["link"];
		$id_subseccion = $this->getNumberSubsections($section) + 1;
		
		$sql = "INSERT INTO subsecciones (id_seccion,id_subseccion,titulo_es,titulo_en,titulo_ca,texto_es,texto_en,texto_ca,link_externo,link)VALUES($section,$id_subseccion,'$titulo_es','$titulo_en','$titulo_ca','$texto_es','$texto_en','$texto_ca','$link_externo','$link')";
		
		echo "<br>$sql<br>";
		if($result = mysql_query($sql, $this->linkdb))
		{	
			return true;
			mysql_free_result($result);
		}
		else
			return false;	
	
	}
	
	function addSection($info)
	{
		$titulo_es = $info["titulo_es"]; 
		$titulo_en = $info["titulo_en"];
		$titulo_ca = $info["titulo_ca"];
		$texto_es = $info["texto_es"];
		$texto_en = $info["texto_en"];
		$texto_ca = $info["texto_ca"];
		$link_externo = $info["link_externo"];
		$link = $info["link"];
		
		$id_seccion = $this->getNumberSections() + 1;
		
		$sql = "INSERT INTO secciones (id_seccion,titulo_es,titulo_en,titulo_ca,texto_es,texto_en,texto_ca,link_externo,link)VALUES($id_seccion,'$titulo_es','$titulo_en','$titulo_ca','$texto_es','$texto_en','$texto_ca','$link_externo','$link')";
		
		if($result = mysql_query($sql, $this->linkdb))
			return true;
			
		else
			return false;	
	
			
	}
	
	function removeSection($section)
	{
		$sql = "DELETE FROM secciones WHERE id_seccion='$section'";
		$sql2 = "DELETE FROM subsecciones WHERE id_seccion='$section'";
		
		if($result = mysql_query($sql, $this->linkdb)&& $result2 = mysql_query($sql2, $this->linkdb))
		{	
			return true;
			mysql_free_result($result);
		}
		else
			return false;	
	
	}
	
	
	function removeSubsection($section, $subsection)
	{
		$sql = "DELETE FROM subsecciones WHERE id_subseccion='$subsection' AND id_seccion='$section'";
		
		
		if($result = mysql_query($sql, $this->linkdb))
		{	
			return true;
			mysql_free_result($result);
		}
		else
			return false;	
	
	}
	
		
	
	
 }
 

?>
