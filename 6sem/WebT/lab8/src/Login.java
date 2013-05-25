

import java.io.IOException;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.sql.*;
import java.text.SimpleDateFormat;
import java.util.Calendar;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Servlet implementation class Login
 */
@WebServlet("/Login")
public class Login extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public Login() {
        super();
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		ServletContext sc = this.getServletContext();	
		RequestDispatcher rd = sc.getRequestDispatcher("/login.jsp");	
		rd.include(request, response);	
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String db_url = "jdbc:mysql://localhost:3306/sem6_java_users";
		String nickname = request.getParameter("nickname");
		String password = request.getParameter("password");
		String encrypted_password_str = nickname.substring(0, 2) + password +
				                        nickname.substring(nickname.length() - 2, nickname.length());
				
		MessageDigest m = null;
		try {
			m = MessageDigest.getInstance("MD5");
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		}
		m.reset();
		m.update(encrypted_password_str.getBytes());
		byte[] digest = m.digest();
		BigInteger bigInt = new BigInteger(1,digest);
		String encrypted_password = bigInt.toString(16);
		// Now we need to zero pad it if you actually want the full 32 chars.
		while(encrypted_password.length() < 32 ){
			encrypted_password = "0" + encrypted_password;
		}	
		
		
	    try {
			Connection con = DriverManager.getConnection(db_url, "", "");
			PreparedStatement q_stmt = con.prepareStatement("SELECT `users`.* FROM `users`" +
							"WHERE `users`.`nickname` = ? AND `users`.`encrypted_password` = ?");
			q_stmt.setString(1, nickname);
			q_stmt.setString(2, encrypted_password);
			ResultSet rs = q_stmt.executeQuery();
			if ( !rs.first() )
				throw new Exception("Wrong nickname/password");
			request.getSession().setAttribute("nickname", nickname);
			
			Calendar cal = Calendar.getInstance();
			SimpleDateFormat sdf = new SimpleDateFormat("d.M.y HH:mm:ss");
			request.getSession().setAttribute("login_at", sdf.format(cal.getTime()));
			response.sendRedirect("/lab8/Counter");
	    } catch (Exception ex) {
			request.setAttribute("e_msg", ex.getMessage());
			ServletContext sc = this.getServletContext();	
			RequestDispatcher rd = sc.getRequestDispatcher("/login.jsp");	
			rd.include(request, response);	
	    }
	}

}
