
<?php
	
	$cms = new contentManager();
	
	if(isset ($_GET["lang"]) && isset ($_GET["selected"]) )
	{	
		
		$lang = $_GET["lang"];
		$selected = $_GET["selected"];
		$ajax = $_GET["ajax"];
		$sections = $cms->getSections();
		$n = count($sections);
		
		if($n > 0)
			echo "<ul class=\"ul_menu\">";
		for($i = 0; $i < $n; $i++)
		{
			$link = $sections[$i]["id_seccion"];
			switch($lang)
			{
				case "ES":
							$name = $sections[$i]["titulo_es"];
							break;	
				
				case "EN":	$name = $sections[$i]["titulo_en"];
							break;
				
				case "CA":	
							$name = $sections[$i]["titulo_ca"];
							break;
			}
		
			if($sections[$i]["link_externo"] == "S")
			{		
				$href = $sections[$i]["link"];
				$js="";
				
			}	
			else
			{	
				$href="index.php?section=$link";
				$js = "javascript:getContent($link,'');return false";
			}	
				
			
			
			
			if($link == $selected)
			{	
				if($ajax)
					echo urlencode("<li class=\"li_menu\"><a id=\"menu-$link\" href=\"$href\" class=\"a_menu\">$name</a></li>");
				else
					echo "<li class=\"li_menu\"><a id=\"menu-$link\" href=\"$href\" class=\"a_menu_sel\">$name</a></li>";
			}
			else
			{	
				if($ajax)	
					echo urlencode("<li class=\"li_menu\"><a id=\"menu-$link\" href=\"$href\" class=\"a_menu\">$name</a></li>");
				else
					echo"<li class=\"li_menu\"><a id=\"menu-$link\" href=\"$href\" class=\"a_menu\">$name</a></li>";	
			}
		}
		
		if($n > 0)
		echo "</ul>";
	}	

?>