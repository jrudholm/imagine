#!/bin/sh

api=api.h

sum=$( sed '/.*__IMAGINE_API_MD5.*/d' < $api | md5sum | awk '{print $1}' )

sed -i 's/\(.*__IMAGINE_API_MD5\).*/\1\t"'$sum'"/' $api
