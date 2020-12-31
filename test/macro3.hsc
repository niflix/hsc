<$macro foo /CLOSE bar:string="bar_foo" foo:string="foo_foo" fox:string="fox_foo">
  bar(foo)=<(bar)> foo(foo)=<(foo)> fox(foo)=<(fox)>
  <$content>
  bar(foo)=<(bar)> foo(foo)=<(foo)> fox(foo)=<(fox)>
</$macro>

<$define bar:string="bar_global">
<$define fox:string="fox_global">

<html>
  <head>
    <title>macro3</title>
  </head>
  <body>
    <h1>foo 1</h1>
    <foo></foo>
    <h1>foo 2</h1>
    <foo><$define foo:string="foo_content"></foo>
  </body>
</html>
