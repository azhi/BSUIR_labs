IF OBJECT_ID ('dbo.getDepartmentCountByGroup', 'FN') IS NOT NULL
    DROP FUNCTION dbo.getDepartmentCountByGroup;
GO

CREATE FUNCTION dbo.getDepartmentCountByGroup(@groupName AS nvarchar(50))
RETURNS bigint
BEGIN
  RETURN(
    SELECT count(*) FROM HumanResources.Department WHERE GroupName = @groupName
  )
END
GO

SELECT dbo.getDepartmentCountByGroup('Manufacturing')
SELECT dbo.getDepartmentCountByGroup('Executive General and Administration')
GO
