//constructora de objeto ajax
function newAjax()
{ 
	var xmlhttp=false; 
	try 
	{ 
		// Creación del objeto ajax para navegadores diferentes a Explorer 
		xmlhttp = new ActiveXObject("Msxml2.XMLHTTP"); 
	} catch (e) { 
					// o bien 
					try 
					{ 
						// Creación del objet ajax para Explorer 
						xmlhttp = new ActiveXObject("Microsoft.XMLHTTP"); } catch (E) { 
						xmlhttp = false; 
					} 
				} 

				if (!xmlhttp && typeof XMLHttpRequest!='undefined') 
				{ 
					xmlhttp = new XMLHttpRequest(); 
				} 
				
	return xmlhttp; 
} 

function getSections()
{
	divMenu = document.getElementById('menu');
	ajaxsect = newAjax();
	
	ajaxsect.open("GET", "ajaxInterface.php?action=loadsect",true); 
  	ajaxsect.onreadystatechange = function() 
	{ 
  		if (ajaxsect.readyState==4) 
		{ 
			cadena = unescape(ajaxsect.responseText);
			cadenafinal=cadena.replace(/\+/gi," ");
			divMenu.innerHTML = cadenafinal;
     	} 
  	}
	ajaxsect.send(null)
}

function getSubsections(seccion)
{
	divSubseccion = document.getElementById('subseccion');
	ajaxsub = newAjax();
	ajaxsub.open("GET", "ajaxInterface.php?action=loadsub&section="+ seccion,true); 
  	
	ajaxsub.onreadystatechange = function() 
	{ 
  		if (ajaxsub.readyState==4) 
		{ 
			//alert(ajaxsub.responseText);
			cadena = unescape(ajaxsub.responseText);
			//alert(cadena);
			cadenafinal=cadena.replace(/\+/gi," ");
			//alert(cadenafinal);
			divSubseccion.innerHTML = cadenafinal;
     	} 
  	}
	ajaxsub.send(null) 
}

// obtiene el contenido y las subsecciones de cada seccion.
function getContent(seccion,subseccion)
{
	
	divContenido = document.getElementById('contenido');  
  	ajaxcont = newAjax();
	// ajax que obtiene el contenido de la seccion,subseccion contenido
	
 	//alert("subseccion:" + subseccion);
	ajaxcont.open("GET", "ajaxInterface.php?action=loadcont&section="+ seccion + "&subsection="+ subseccion,true);
	
  	ajaxcont.onreadystatechange = function() 
	{ 
  		if (ajaxcont.readyState==4) 
		{ 
   		
			if(subseccion == "")
			{	
				getSubsections(seccion);
				changeSelected("menu",seccion);
			}
			else
			{	
				changeSelected("subsect",subseccion);
			}
			
			
			cadena = unescape(ajaxcont.responseText);
			
			cadenafinal=cadena.replace(/\+/gi," ");
			
			divContenido.innerHTML = cadenafinal;
     	} 
  	}
				
	
	
  			
  	
	
  	ajaxcont.send(null)
	
}
	  
//cambia el idioma de la pagina
function changeLang(lang)
{
		ajaxlang = newAjax();
		ajaxlang.open("GET","ajaxInterface.php?action=changelang&lang="+lang,true);
		ajaxlang.onreadystatechange = function() 
		{ 
  			if (ajaxlang.readyState==4) 
			{ 
				getCurrentContent();
     		} 
  		} 
		
		ajaxlang.send(null);
}

function getCurrentContent()
{
	divSubseccion = document.getElementById('subseccion');
	divContenido = document.getElementById('contenido');
	divMenu = document.getElementById('menu');
    
	ajaxcont = newAjax();
	ajaxsub = newAjax();
	ajaxsect = newAjax();
  	
	// ajax que obtiene el contenido de la seccion,subseccion contenido	
 	ajaxcont.open("GET", "ajaxInterface.php?action=loadcurcont",true); 
  	ajaxcont.onreadystatechange = function() 
	{ 
  		if (ajaxcont.readyState==4) 
		{ 
   			cadena = unescape(ajaxcont.responseText);
			cadenafinal=cadena.replace(/\+/gi," ");
			divContenido.innerHTML = cadenafinal;
     	} 
  	}
	
	
	ajaxsect.open("GET", "ajaxInterface.php?action=loadsect",true); 
  	ajaxsect.onreadystatechange = function() 
	{ 
  		if (ajaxsect.readyState==4) 
		{ 
			cadenasect = unescape(ajaxsect.responseText);
			cadenasect =cadenasect.replace(/\+/gi," ");
			divMenu.innerHTML = cadenasect;
     	} 
  	}
				
	// ajax que obtiene el contenido de 
	ajaxsub.open("GET", "ajaxInterface.php?action=loadcursubsect",true); 
  	
	ajaxsub.onreadystatechange = function() 
	{ 
  		if (ajaxsub.readyState==4) 
		{ 
			cadenasub = unescape(ajaxsub.responseText);
			cadenasub=cadenasub.replace(/\+/gi," ");
			divSubseccion.innerHTML = cadenasub;
     	} 
  	} 
  			
  	ajaxsub.send(null) 
  	ajaxcont.send(null)
	ajaxsect.send(null)
}

function changeSelected(prefix, index)
{
	elements = document.getElementsByTagName('a');
	for(i=0;i<elements.length;i++)
	{
		element = elements[i];
		//alert(element.id);
		
		aux = element.id.split("-");
		//alert("id:"+element.id+" aux[0]:"+ aux[0] +" prefix:"+ prefix+" "+"aux[1]:"+ aux[1]+ " index:" + index);
		if(prefix == "menu")
		{
			if (aux[0] == prefix)
			{
				if(aux[1] == index)
					element.className = "menusel";
				else
					element.className = "menuunsel";
			}
		}
		else if(prefix =="subsect")
		{
			
			if (aux[0] == prefix)
			{
				//alert("prefix:"+aux[0]+ " index:"+aux[1]);
				if(aux[1] == index)
					element.className = "subsectionsel";
				
				else
					element.className = "subsectionunsel";
			}	
		}
	}
	

}