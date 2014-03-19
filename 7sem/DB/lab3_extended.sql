IF OBJECT_ID('dbo.Person', 'U') IS NOT NULL DROP TABLE dbo.Person

DECLARE @source_table_schema nvarchar(255)
DECLARE @source_table_name nvarchar(255)
DECLARE @dest_table_schema nvarchar(255)
DECLARE @dest_table_name nvarchar(255)

DECLARE @source_table_full_name nvarchar(255)
DECLARE @dest_table_full_name nvarchar(255)
DECLARE @row BIGINT
DECLARE @name NVARCHAR(255)
DECLARE @definition NVARCHAR(255)
DECLARE @c_name NVARCHAR(255)

SELECT @source_table_schema = 'Person'
SELECT @source_table_name = 'Person'
SELECT @source_table_full_name = @source_table_schema + '.' + @source_table_name
SELECT @dest_table_schema = 'dbo'
SELECT @dest_table_name = 'Person'
SELECT @dest_table_full_name = @dest_table_schema + '.' + @dest_table_name

EXEC('SELECT TOP 0 * INTO ' + @dest_table_full_name + ' FROM ' + @source_table_full_name)

DECLARE @PKColsTbl table ([name] NVARCHAR(255), col NVARCHAR(255))
INSERT INTO @PKColsTbl
  SELECT  c.Constraint_Name, c.COLUMN_NAME
  FROM    INFORMATION_SCHEMA.TABLE_CONSTRAINTS pk
  JOIN    INFORMATION_SCHEMA.KEY_COLUMN_USAGE c ON c.TABLE_NAME = pk.TABLE_NAME
                                                   and c.CONSTRAINT_NAME = pk.CONSTRAINT_NAME
  WHERE   pk.TABLE_SCHEMA = @source_table_schema
          and pk.TABLE_NAME = @source_table_name
          and pk.CONSTRAINT_TYPE = 'PRIMARY KEY'

DECLARE @pkCol NVARCHAR(255)
SELECT @pkCol = stuff((select ',' + col from @PKColsTbl for XML PATH('')), 1,1,'')
EXEC('ALTER TABLE ' + @dest_table_name + ' ADD CONSTRAINT ' + 'PK_' + @dest_table_name + ' PRIMARY KEY (' + @pkCol + ')')

DECLARE @CK_constrs table ([id] bigint identity(1, 1), [name] NVARCHAR(255), [definition] NVARCHAR(255), [c_name] NVARCHAR(255))
INSERT INTO @CK_constrs
  SELECT cc.name, cc.definition, c.name
    FROM sys.check_constraints cc
    JOIN sys.columns c ON c.column_id = cc.parent_column_id AND c.object_id = OBJECT_ID(@source_table_full_name)
    WHERE PARENT_OBJECT_ID = OBJECT_ID(@source_table_full_name)
SELECT @row = MIN(id) FROM @CK_constrs
WHILE (isNull(@row, 0) <> 0)    -- returns null when no more rows, which gets converted to 0 (zero)
BEGIN
  SELECT @name = [name], @definition = [definition], @c_name = [c_name] FROM @CK_constrs where id = @row
  EXEC('ALTER TABLE ' + @dest_table_full_name + ' ADD CONSTRAINT ' + @name + ' CHECK' + @definition)

  SELECT @row = min(id) FROM @CK_constrs WHERE id > @row
END

DECLARE @DF_constrs table ([id] bigint identity(1, 1), [name] NVARCHAR(255), [definition] NVARCHAR(255), [c_name] NVARCHAR(255))
INSERT INTO @DF_constrs
  SELECT dc.name, dc.definition, c.name
    FROM sys.default_constraints dc
    JOIN sys.columns c ON c.column_id = dc.parent_column_id AND c.object_id = OBJECT_ID(@source_table_full_name)
    WHERE PARENT_OBJECT_ID = OBJECT_ID(@source_table_full_name)
SELECT @row = MIN(id) FROM @DF_constrs
WHILE (isNull(@row, 0) <> 0)    -- returns null when no more rows, which gets converted to 0 (zero)
BEGIN
  SELECT @name = [name], @definition = [definition], @c_name = [c_name] FROM @DF_constrs where id = @row
  EXEC('ALTER TABLE ' + @dest_table_full_name + ' ADD CONSTRAINT ' + @name + ' DEFAULT' + @definition + 'FOR ' + @c_name)

  SELECT @row = min(id) FROM @DF_constrs WHERE id > @row
END