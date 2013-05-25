<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
<style>
  #main {
    width: 400px;
    margin-left: auto;
    margin-right: auto;
  }
  
  div.input_wrapper {
    position: relative;
    margin: 10px 0;
  }
  
  form input[type="text"] {
    position: absolute;
    right: 0;
  }
  
  form input[type="submit"] {
    padding: 5px 30px;
  }
  
  p.error {
    color: #f00;
    font-size: 150%;
  }
  
  p.success {
    color: #0f4;
    font-size: 150%;
  }
</style>
</head>
<body>
  <div id="main">
    <% if (request.getAttribute("e_msg") != null) { %>
      <p class="error"><%= request.getAttribute("e_msg") %></p>
    <% } %>
      
    <% if (request.getAttribute("s_msg") != null) { %>
      <p class="success"><%= request.getAttribute("s_msg") %></p>
    <% } %>
    
	<form action="<%= request.getRequestURI() %>" method="post">
	  <h2> FROM </h2>
	  <div class="input_wrapper"><label> Bank name: <input type="text" name="from_bank_name" /></label></div>
	  <div class="input_wrapper"><label> Account number: <input type="text" name="from_account_no" /></label></div>
	
	  <h2> TO </h2>
	  <div class="input_wrapper"><label> Bank name: <input type="text" name="to_bank_name" /></label></div>
	  <div class="input_wrapper"><label> Account number: <input type="text" name="to_account_no" /></label></div>
	  
	  <h2> MONEY </h2>
	  <div class="input_wrapper"><label> Amount: <input type="text" name="amount" /></label></div>
	  <input type="submit" value="transfer" />
	</form>
  </div>
</body>
</html>