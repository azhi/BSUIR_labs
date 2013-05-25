<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Count !</title>
</head>
<body>
  <p> Hello, <%= request.getSession().getAttribute("nickname").toString() %>! </p>
  <form action="<%= request.getRequestURI() %>" method="post" enctype="multipart/form-data" > 
   <label> Poems file: <input type="file" name="file" /> </label> <br />
   <input type="submit" value="Count!" />
  </form>
</body>
</html>