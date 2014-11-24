
<?php
	
	$cms = new ContentManager();
	

	if( isset($_GET["lang"]) && isset($_GET["section"]) )
	{	
		$lang = $_GET["lang"];
		$section = $_GET["section"];
		$selected = $_GET["selected"];
		$ajax = $_GET["ajax"];
		$subsections = $cms->getSubsections($section);
		
		
		
		$n = count($subsections);
		
		if($n > 0)
		{	
			echo"<p class=\"separador_fondo\"> </p>";
			echo "<ul class=\"ul_submenu\">";
		}
		else
			echo"<p class=\"separador\"> </p>";
		for($i = 0; $i < $n ; $i++)
		{
			$link = $subsections[$i]["id_subseccion"];
			switch($lang)
			{
				case "ES":
							$name = $subsections[$i]["titulo_es"];
							break;	
				
				case "EN":	$name = $subsections[$i]["titulo_en"];
							break;
				
				case "CA":	
							$name = $subsections[$i]["titulo_ca"];
							break;
			}
			
			if($subsections[$i]["link_externo"] == "S")
			{	
				$href = $subsections[$i]["link"];
				$js="";
			}	
			
			else
			{	
				$js = "javascript:getContent($section,$link);return false";
				$href="index.php?section=$section&amp;subsection=$link";
			}	
				
			
			
			
			if($link == $selected)
			{	
				if($ajax)
					echo urlencode("<li class=\"li_submenu\"><a id=\"subsect-$link\"href=\"$href\" class=\"enllac_sel\">$name</a></li>");
				else
					echo "<li class=\"li_submenu\"><a id=\"subsect-$link\" href=\"$href\" class=\"enllac_sel\">$name</a></li>";	
			}
			else
			{	
				if($ajax)	
					echo urlencode("<li class=\"li_submenu\"><a id=\"subsect-$link\"href=\"$href\" class=\"enllac\">$name</a></li>");
				else
					echo "<li class=\"li_submenu\"><a id=\"subsect-$link\" href=\"$href\" class=\"enllac\">$name</a></li>";	
			}
		}
		
		if($n > 0)
		{
			echo "</ul>";
			echo"<p class=\"separador_fondo\"> </p>";
		}	
		else
			echo"<p class=\"separador\"> </p>";
	}	

?>