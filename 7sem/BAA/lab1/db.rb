require 'mysql2'
require 'yaml'

class DB
  @@options = YAML.load(File.read('db.yml'))
  @@connection = Mysql2::Client.new(@@options)
  class << self
    def query(sql)
      @@connection.query(sql)
    end
  end
end
