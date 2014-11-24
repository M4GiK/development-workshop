<?
	session_start();
	if($_GET["logout"] == "true")
	{
		session_unregister("admin");
		header("location:../../index.php");
		exit();
	}
	else
	{
		//session_register("admin");
		if(!isset($_SESSION["admin"]))
			header("location:../login.php");
	}	
	
	
?>	


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Web GRID</title>
<link href="../../frontend/style-css/style_admin_usuario.css" rel="stylesheet" type="text/css" />
 <!--[if IE 6]>
        <link rel="stylesheet" type="text/css" href="style-css/style_ie6_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]--> 
 <!--[if IE 7]>
        <link rel="stylesheet" type="text/css" href="style-css/style_ie7_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]-->
 
<meta http-equiv="content-language" content="cas" />
<!-- TinyMCE -->
<script language="javascript" type="text/javascript" src="../../lib/tinymce/jscripts/tiny_mce/tiny_mce.js"></script>
<script language="javascript" type="text/javascript">
	tinyMCE.init({
		mode : "exact",
		elements: "texto_es,texto_en,texto_ca",
		theme : "advanced",
		plugins : "devkit,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template,ibrowser",
		theme_advanced_buttons1_add_before : "save,newdocument,separator",
		theme_advanced_buttons1_add : "fontselect,fontsizeselect",
		theme_advanced_buttons2_add : "separator,insertdate,inserttime,preview,separator,forecolor,backcolor",
		theme_advanced_buttons2_add_before: "cut,copy,paste,pastetext,pasteword,separator,search,replace,separator",
		theme_advanced_buttons3_add_before : "tablecontrols,separator",
		theme_advanced_buttons3_add : "emotions,iespell,media,advhr,separator,print,separator,ltr,rtl,separator,fullscreen,ibrowser",
		theme_advanced_buttons4 : "insertlayer,moveforward,movebackward,absolute,|,styleprops,|,cite,abbr,acronym,del,ins,attribs,|,visualchars,nonbreaking,template,|,code",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		theme_advanced_path_location : "bottom",
		content_css : "../../frontend/style/style.css",
	    plugin_insertdate_dateFormat : "%Y-%m-%d",
	    plugin_insertdate_timeFormat : "%H:%M:%S",
		extended_valid_elements : "hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
		external_link_list_url : "example_link_list.js",
		external_image_list_url : "example_image_list.js",
		flash_external_list_url : "example_flash_list.js",
		media_external_list_url : "example_media_list.js",
		template_external_list_url : "example_template_list.js",
		file_browser_callback : "fileBrowserCallBack",
		document_base_url : "http://weboptserv.lsi.upc.edu/WEBGRID/",
		relative_urls:true,
		remove_script_host : false,
		theme_advanced_resize_horizontal : false,
		theme_advanced_resizing : true,
		nonbreaking_force_tab : true,
		apply_source_formatting : true,
		template_replace_values : {
			username : "Jack Black",
			staffid : "991234"
		}
	});

	function fileBrowserCallBack(field_name, url, type, win) {
		// This is where you insert your custom filebrowser logic
		alert("Example of filebrowser callback: field_name: " + field_name + ", url: " + url + ", type: " + type);

		// Insert new URL, this would normaly be done in a popup
		win.document.forms[0].elements[field_name].value = "someurl.htm";
	}
</script>
<!-- /TinyMCE -->
</head>
<body>
<?
	include "../config.php";
	include "../../management/contentManager.php";
	include "../../management/incidenceManager.php";
	include "../menus.php";
	
	$action = $_GET["action"];
	$section = $_GET["section"];
	$subsection = $_GET["subsection"];
	
	$cms = new contentManager();
	switch($action)
	{
		case "edit":
					//echo "section:$section subsection:$subsection<br/>";
					$info = $cms->getSection($section, $subsection);
					//print_r($info);			
					break;
		case "delete":
						break;
		case "newSection":
							break;
		
		case "newSubsection":
								break;												
	}
	
	$im = new incidenceManager();
	$num = $im->getNumberWaitingIncidences();

	
?>
<div id="contenido">
	<div id="contenido">
	<div id="cabecera">
		<div id="logo"><? include "../header.htm"?></div>
	</div>
	<div id="central">
		<div id="primer_menu">
			<ul class="ul_primermenu">
				<? getFirstMenuGestion();?>
			</ul>
		</div>	
		
	<div id="pestanas"> 
		<ul class="ul_pestana">
			<? getTabsGestion("Contenido",$num);?>
		</ul>
	</div>
	<div id="parte_central">
	<div id="menu_interior">
		<ul class="ul_segundomenu">
			<li class="li_segundomenu"><a href="index.php">Volver</a></li>
		</ul>
	</div>	 
<div id="result" align="center">
<h1 align="center">Editar contenido<br/></h1>
<form id="form1" method="post" action="saveSection.php">
  <table width="80%" cellpadding="0" cellspacing="0" class="tabladatos">
    <tr>
      <th class="th_tabladatos" scope="row"><strong>titulo_es</strong></td>
      <td class="td_tabladatos_PAR"><input type="text" name="titulo_es" value="<? echo $info["titulo_es"];?>" /> 
      <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>titulo_en</strong></td>
      <td class="td_tabladatos_PAR"><input type="text" name="titulo_en" value="<? echo $info["titulo_en"]; ?>" />
      <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>titulo_ca</strong></td>
      <td class="td_tabladatos_PAR"><input type="text" name="titulo_ca" value="<? echo $info["titulo_ca"];?>"/>
          <br /></td>
    </tr>
	 <tr>
      <th class="th_tabladatos" scope="row"><strong>link externo </strong></td>
      <td valign="top" class="td_tabladatos_PAR">
        
          <? 
		  	
			
			$linkhtml ="<label>";
            
			if($info["link_externo"] == "S")
				$linkhtml .="<input name=\"link_externo\" type=\"radio\" value=\"S\" checked=\"checked\" />";
			else
				$linkhtml .="<input name=\"link_externo\" type=\"radio\" value=\"S\" />";	
				 
            $linkhtml.="S&iacute;</label>
          					<br />
         				 <label>";
			
			if($info["link_externo"] == "N")
				$linkhtml .="<input name=\"link_externo\" type=\"radio\" value=\"N\" checked=\"checked\" />";
			else
				$linkhtml .="<input name=\"link_externo\" type=\"radio\" value=\"N\" />";	
						 
             $linkhtml.="No</label>";
			 echo $linkhtml;
		?>
          <br />
          <br />
        url:
        <input name="link" type="text" id="link" value="<? echo $info["link"]; ?>"/></td></tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>texto_es</strong></td>
      <td valign="top" class="td_tabladatos_PAR"><textarea name="texto_es" id="texto_es" cols="80" rows="10"><? echo $info["texto_es"];?></textarea>
      <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>texto_en</strong></td>
      <td valign="top" class="td_tabladatos_PAR"><textarea name="texto_en" id="texto_en" cols="80" rows="10"><? echo $info["texto_en"];?></textarea>
      <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>texto_ca</strong></td>
      <td valign="top" class="td_tabladatos_PAR"><textarea name="texto_ca" id="texto_ca" cols="80" rows="10"><? echo $info["texto_ca"];?></textarea>
      <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>fecha modificacion </strong></td>
      <td class="td_tabladatos_PAR"><br />
      <? echo $info["fecha_modif"];?>
	  <input type="hidden" name="section" value="<? echo $section;?>"/>
	  <input type="hidden" name="subsection" value="<? echo $subsection;?>"/>
	  <input type="hidden" name="action" value="<? echo $action;?>"/></td>
    </tr>
    <tr>
      <td colspan="2" class="td_tabladatos_PAR"><input type="submit" name="Submit" value="guardar" /></td>
    </tr>
  </table>

</form>
</div>
<br />
<br />

	</div>
</div>
<div id="peu">&nbsp;</div>
</div>
</body>
</html>