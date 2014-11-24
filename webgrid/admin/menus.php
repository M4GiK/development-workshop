<?
	$items_gestion = array(
							array("name" =>"Ejecuciones","link" =>"../executions/index.php" ),
							array("name" =>"Incidencias","link" =>"../incidences/index.php" ),
							array("name" =>"Usuarios","link" =>"../users/index.php" ),
							array("name" =>"Eventos","link" =>"../events/index.php" ),
							array("name" =>"Noticias","link" =>"../news/index.php" ),
							array("name" =>"Contenido","link" =>"../content/index.php"));
	$items_config = array(
							array("name" =>"Max ejecuciones","link" => "max_executions.php" ),
							array("name" =>"Password","link" =>"password.php" ),
							array("name" =>"Mail","link" =>"mail.php" ),
							array("name" =>"Avisos","link" =>"avisos.php" ),
							array("name" =>"Via ejecucion","link" =>"via.php" ),
							array("name" =>"Mensaje admin","link" =>"messageAdmin.php" ));
	
	$items_first_menu_gestion = array(
								array("name" =>"Configuraci&oacute;n","link" => "../configuration/max_executions.php" ),
								array("name" =>"Inicio","link" =>"../../index.php?section=1" ),
								array("name" =>"logout","link" => "$PHP_SELF?logout=true" ));
	
	$items_first_menu_config = array(
										array("name" =>"Gesti&oacute;n","link" => "../executions/index.php" ),
										array("name" =>"Inicio","link" =>"../../index.php?section=1" ),
										array("name" =>"logout","link" => "$PHP_SELF?logout=true" ));
							
	
							
							
function getTabsGestion($selected,$num_incidencias)
{
	global $items_gestion;
	for ($i = 0 ; $i < count($items_gestion); $i++)
	{
		$name = $items_gestion[$i]["name"];
		$link = $items_gestion[$i]["link"];
		if($name == $selected)
			echo "<li class=\"pestana_selec\"><strong>$name</strong></li>";
		
		else
		{
			if($name == "Incidencias")
				echo "<li class=\"pestanas\"><a href=\"$link\">Incidencias($num_incidencias)</a></li>";
			
			else
					echo "<li class=\"pestanas\"><a href=\"$link\">$name</a></li>";		
		}	
	}
}

function getTabsConfig($selected)
{
	global $items_config;
	for ($i = 0 ; $i < count($items_config); $i++)
	{
		$name = $items_config[$i]["name"];
		$link = $items_config[$i]["link"];
		
		if($name == $selected)
			echo "<li class=\"pestana_selec\"><strong>$name</strong></li>";
		
		else
			echo "<li class=\"pestanas\"><a href=\"$link\">$name</a></li>";		
		
	}
}

function getFirstMenuGestion()
{
	
	global $items_first_menu_gestion;
	for($i = 0; $i < count($items_first_menu_gestion); $i++)
	{
		$name = $items_first_menu_gestion[$i]["name"];
		$link = $items_first_menu_gestion[$i]["link"];
		
		echo "<li class=\"li_primermenu\"><a href=\"$link\" class=\"link_primermenu\">$name</a></li>";
		
	}
}


function getFirstMenuConfig()
{
	global $items_first_menu_config;
	for($i = 0; $i < count($items_first_menu_config); $i++)
	{
		$name = $items_first_menu_config[$i]["name"];
		$link = $items_first_menu_config[$i]["link"];
		
		echo "<li class=\"li_primermenu\"><a href=\"$link\" class=\"link_primermenu\">$name</a></li>";
		
	}
}

?>