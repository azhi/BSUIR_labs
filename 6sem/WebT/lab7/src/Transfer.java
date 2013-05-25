
import java.sql.*;
import java.text.SimpleDateFormat;
import java.util.Calendar;

import java.io.IOException;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Servlet implementation class Transfer
 */
@WebServlet("/Transfer")
public class Transfer extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public Transfer() {
        super();
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		ServletContext sc = this.getServletContext();	
		RequestDispatcher rd = sc.getRequestDispatcher("/transfer.jsp");	
		rd.include(request, response);	
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String from_bank_name = request.getParameter("from_bank_name");
		String from_url = "jdbc:mysql://localhost:3306/sem6_java_bank_" + from_bank_name;
		
		String to_bank_name = request.getParameter("to_bank_name");
		String to_url = "jdbc:mysql://localhost:3306/sem6_java_bank_" + to_bank_name;
		
		long from_account_no = Long.parseLong(request.getParameter("from_account_no"));
		long to_account_no = Long.parseLong(request.getParameter("to_account_no"));
		long amount = Long.parseLong(request.getParameter("amount"));
		
		Savepoint from_save_point = null;
		Savepoint to_save_point = null;
		Connection from_con = null;
		Connection to_con = null;
	    try {
	      from_con = DriverManager.getConnection(from_url, "", "");
	      to_con = DriverManager.getConnection(to_url, "", "");
	      
	      PreparedStatement q_stmt = from_con.prepareStatement("SELECT `accounts`.`amount` FROM `accounts`" +
	                         "WHERE `accounts`.`account_no` = ?");
	      q_stmt.setLong(1, from_account_no);
	      ResultSet rs = q_stmt.executeQuery();
	      if ( !rs.first() )
	    	  throw new WrongTransaction("Account with your account number not found.");
	      long money_left = rs.getLong("amount");
	      if ( money_left < amount )
	    	  throw new WrongTransaction("You don't have enough founds!");
	      
	      q_stmt = to_con.prepareStatement("SELECT `accounts`.`amount` FROM `accounts`" +
	                         "WHERE `accounts`.`account_no` = ?");
	      q_stmt.setLong(1, to_account_no);
	      rs = q_stmt.executeQuery();
	      if ( !rs.first() )
	    	  throw new WrongTransaction("Recipient doesn't exists!");
	      
	      money_left -= amount;
	      from_save_point = from_con.setSavepoint();
	      to_save_point = to_con.setSavepoint();
	      q_stmt = from_con.prepareStatement("UPDATE `accounts` " +
	                         "SET `accounts`.`amount` = ? " +
	                         "WHERE `accounts`.`account_no` = ?");
	      q_stmt.setLong(1, money_left);
	      q_stmt.setLong(2, from_account_no);
	      q_stmt.executeUpdate();
	      
	      q_stmt = to_con.prepareStatement("SELECT `accounts`.`amount` FROM `accounts`" +
	                         "WHERE `accounts`.`account_no` = ?");
	      q_stmt.setLong(1, to_account_no);
	      rs = q_stmt.executeQuery();
	      if ( !rs.first() )
	    	  throw new WrongTransaction("Recipient doesn't exists!");
	      long to_amount = rs.getLong("amount");
	      
	      q_stmt = to_con.prepareStatement("UPDATE `accounts` " +
	                         "SET `accounts`.`amount` = ? " +
	                         "WHERE `accounts`.`account_no` = ?");
	      q_stmt.setLong(1, to_amount + amount);
	      q_stmt.setLong(2, to_account_no);
	      q_stmt.executeUpdate();
	      
	      Calendar cal = Calendar.getInstance();
	      SimpleDateFormat sdf = new SimpleDateFormat("d.M.y HH:mm:ss");
          request.setAttribute("s_msg", "Transfer successfull (" + amount + "$ at " + sdf.format(cal.getTime()) + ")");
          
          from_con.close();
          to_con.close();
	    } catch (Exception ex) {
          request.setAttribute("e_msg", ex.getMessage());
          if ( from_save_point != null )
			try {
				from_con.rollback(from_save_point);
			} catch (SQLException e) {
		        request.setAttribute("e_msg", request.getAttribute("e_msg") + "\nBad thing happened: " + e.getMessage());
			}
          if ( to_save_point != null )
			try {
				to_con.rollback(to_save_point);
			} catch (SQLException e) {
		        request.setAttribute("e_msg", request.getAttribute("e_msg") + "\nBad thing happened: " + e.getMessage());
			}
		}
	    
		ServletContext sc = this.getServletContext();	
		RequestDispatcher rd = sc.getRequestDispatcher("/transfer.jsp");	
		rd.include(request, response);	
	}

}
