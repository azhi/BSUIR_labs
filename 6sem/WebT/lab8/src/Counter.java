

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.List;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.annotation.MultipartConfig;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.Part;

/**
 * Servlet implementation class Counter
 */
@WebServlet("/Counter")
@MultipartConfig
public class Counter extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public Counter() {
        super();
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		ServletContext sc = this.getServletContext();	
		RequestDispatcher rd = sc.getRequestDispatcher("/counter.jsp");	
		rd.include(request, response);	
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		
		Part filePart = request.getPart("file");
	    InputStream filecontent = filePart.getInputStream();
	    BufferedReader br = new BufferedReader(new InputStreamReader(filecontent));	
	    
	    List<String> poems = new ArrayList<String>();
	    String currentPoem = "";
	    String strLine;
	    while ((strLine = br.readLine()) != null) {
			if ( strLine.matches("\\*+") ) {
				poems.add(currentPoem);
				currentPoem = "";
			}
			else
				currentPoem += strLine + "\n";
	    } 
	    
	    if (!currentPoem.isEmpty())
			poems.add(currentPoem);
	    
	    String exclamationPoem = null;
	    int mostExcl = -1;
	    for ( String poem : poems) {
	    	int count = poem.split("!", -1).length - 1;
	    	if ( count > mostExcl ) {
	    		mostExcl = count;
	    		exclamationPoem = poem;
	    	}
	    }
	    String res = exclamationPoem.substring(0, Math.min(4000, exclamationPoem.length())); 
	    
	    PrintWriter out = response.getWriter();
	    Cookie[] cookies = request.getCookies();
	    boolean foundCookie = false;

	    for(int i = 0; i < cookies.length; i++) {
	      Cookie cookie = cookies[i];
	      if (cookie.getName().equals("res")) {
	        cookie.setValue(res);
	        response.addCookie(cookie);
	        foundCookie = true;
	      }
	    }

	    if (!foundCookie) {
	      Cookie cookie = new Cookie("res", URLEncoder.encode(res, "UTF-8"));
	      cookie.setMaxAge(60*60);
	      response.addCookie(cookie);
	    }

	    out.println(exclamationPoem); 
	}

}
